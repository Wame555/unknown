#include "binancewebsocket.h"
#include <nlohmann/json.hpp>
#include <QDebug>
#include <QStringList>

// Kis segédfüggvény: QString -> std::string (UTF-8)
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
            QString symbol = QString::fromStdString(
                                 data["stream"].get<std::string>())
                                 .split('@')
                                 .first()
                                 .toUpper();

            // Ár
            double price = std::stod(data["data"]["c"].get<std::string>());

            emit priceUpdated(symbol, price);
        } catch (const std::exception& e) {
            qDebug() << "Error parsing message:" << e.what();
        }
    });
}

void BinanceWebsocket::connectToBinance(const QStringList& symbols) {
    // pl. ["BTCUSDT","ETHUSDT"] -> "btcusdt@ticker/ethusdt@ticker"
    QStringList streams;
    for (const auto& s : symbols) {
        streams << (s.toLower() + "@ticker");
    }
    const QString streamJoined = streams.join('/');

    const QString url = "wss://stream.binance.com:9443/stream?streams=" + streamJoined;

    // ix::WebSocket::setUrl std::string-et vár
    webSocket.setUrl(qstr(url));
    webSocket.start();
}
