#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include "demoaccount.h"
#include "binancewebsocket.h"
#include "telegramnotifier.h"
#include "configmanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void updatePrice(const QString& symbol, double price);
    void onAddPositionClicked();
    void onClosePositionClicked();

private:
    QTableWidget* priceTable;
    QTableWidget* positionsTable;
    QTextEdit* debugConsole;
    DemoAccount* demoAccount;
    BinanceWebsocket* websocket;
    TelegramNotifier* telegramNotifier;
    ConfigManager* configManager;
    void setupUI();
};
