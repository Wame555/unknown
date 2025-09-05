#include "demoaccount.h"

DemoAccount::DemoAccount(QObject* parent) : QObject(parent) {}

void DemoAccount::addPosition(const QString& symbol, double entryPrice, double amount, double tpPrice, double slPrice) {
    positions[symbol] = {entryPrice, amount, tpPrice, slPrice};
    emit pnlUpdated(symbol, 0.0);
}

void DemoAccount::closePosition(const QString& symbol) {
    if (positions.contains(symbol)) {
        positions.remove(symbol);
        emit positionClosed(symbol);
    }
}

double DemoAccount::calculatePnL(const QString& symbol, double currentPrice) {
    if (!positions.contains(symbol)) return 0.0;
    const Position& pos = positions[symbol];
    return (currentPrice - pos.entryPrice) * pos.amount;
}

