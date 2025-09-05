#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <ixwebsocket/IXWebSocket.h>

class BinanceWebsocket : public QObject {
    Q_OBJECT
public:
    explicit BinanceWebsocket(QObject* parent = nullptr);
    Q_INVOKABLE void connectToBinance(const QStringList& symbols);

signals:
    void priceUpdated(const QString& symbol, double price);

private:
    ix::WebSocket webSocket;
};
