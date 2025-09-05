#include "binancewebsocket.h"
#include <nlohmann/json.hpp>
#include <QDebug>
#include <QStringList>

// QString -> std::string (UTF-8)
static inline std::string qstr(const QString& s) {
    const QByteArray u = s.toUtf8();
    return std::string(u.constData(), static_cast<size_t>(u.size()));
}

BinanceWebsocket::BinanceWebsocket(QObject* parent) : QObject(parent) {
    webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        try {
            if (!msg || msg->str.empty()) return;

            auto data = nlohmann::json::parse(msg->str);

            // "btcusdt@ticker" -> "BTCUSDT"
            const auto streamStd = data["stream"].get<std::string>();
            const QString symbol =
                QString::fromStdString(streamStd).split('@').first().toUpper();

            const double price = std::stod(data["data"]["c"].get<std::string>());

            emit priceUpdated(symbol, price);
        } catch (const std::exception& e) {
            qDebug() << "Error parsing message:" << e.what();
        }
    });
}

void BinanceWebsocket::connectToBinance(const QStringList& symbols) {
    // ["BTCUSDT","ETHUSDT"] -> "btcusdt@ticker/ethusdt@ticker"
    QStringList streams;
    for (const auto& s : symbols) streams << (s.toLower() + "@ticker");
    const QString streamJoined = streams.join('/');

    const QString url = "wss://stream.binance.com:9443/stream?streams=" + streamJoined;
    webSocket.setUrl(qstr(url));  // ix::WebSocket std::string-et vár
    webSocket.start();
}
