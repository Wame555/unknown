#pragma once
#include <QObject>
#include <ixwebsocket/IXWebSocket.h>

class BinanceWebsocket : public QObject {
    Q_OBJECT
public:
    explicit BinanceWebsocket(QObject* parent = nullptr);
    void connectToBinance(const QStringList& symbols);

signals:
    void priceUpdated(const QString& symbol, double price);

private:
    ix::WebSocket webSocket;
};

