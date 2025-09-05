#pragma once
#include <QObject>
#include <QMap>
#include <QString>

struct Position {
    double entryPrice;
    double amount;
    double tpPrice;
    double slPrice;
};

class DemoAccount : public QObject {
    Q_OBJECT
public:
    explicit DemoAccount(QObject* parent = nullptr);
    void addPosition(const QString& symbol, double entryPrice, double amount, double tpPrice, double slPrice);
    void closePosition(const QString& symbol);
    double calculatePnL(const QString& symbol, double currentPrice);
    QMap<QString, Position> getPositions() const { return positions; }

signals:
    void pnlUpdated(const QString& symbol, double pnl);
    void tpReached(const QString& symbol);
    void slReached(const QString& symbol);
    void positionClosed(const QString& symbol);

private:
    QMap<QString, Position> positions;
};
