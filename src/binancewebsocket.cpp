#include "binancewebsocket.h"
#include <nlohmann/json.hpp>
#include <QDebug>

BinanceWebsocket::BinanceWebsocket(QObject* parent) : QObject(parent) {
    webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        try {
            auto data = nlohmann::json::parse(msg->str);
            QString symbol = QString::fromStdString(data["stream"].get<std::string>()).split('@').first().toUpper();
            double price = std::stod(data["data"]["c"].get<std::string>());
            emit priceUpdated(symbol, price);
        } catch (const std::exception& e) {
            qDebug() << "Error parsing message: " << e.what();
        }
    });
}

void BinanceWebsocket::connectToBinance(const QStringList& symbols) {
    QString stream;
    for (const auto& symbol : symbols) {
        stream += symbol.toLower() + "@ticker/";
    }
    webSocket.setUrl("wss://stream.binance.com:9443/stream?streams=" + stream);
    webSocket.start();
}
