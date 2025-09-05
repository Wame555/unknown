#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    demoAccount = new DemoAccount(this);
    websocket = new BinanceWebsocket(this);
    telegramNotifier = new TelegramNotifier("", "", this);
    configManager = new ConfigManager(this);

    connect(websocket, &BinanceWebsocket::priceUpdated, this, &MainWindow::updatePrice);
    connect(demoAccount, &DemoAccount::pnlUpdated, this, [this](const QString& symbol, double pnl) {
        for (int i = 0; i < positionsTable->rowCount(); ++i) {
            if (positionsTable->item(i, 0)->text() == symbol) {
                QTableWidgetItem* pnlItem = new QTableWidgetItem(QString::number(pnl, 'f', 2));
                pnlItem->setForeground(pnl >= 0 ? Qt::green : Qt::red);
                positionsTable->setItem(i, 4, pnlItem);
            }
        }
    });

    connect(demoAccount, &DemoAccount::tpReached, this, [this](const QString& symbol) {
        telegramNotifier->sendMessage(QString("TP reached for %1!").arg(symbol));
    });

    connect(demoAccount, &DemoAccount::slReached, this, [this](const QString& symbol) {
        telegramNotifier->sendMessage(QString("SL reached for %1!").arg(symbol));
    });

    setupUI();
    websocket->connectToBinance({"btcusdt", "solusdt", "ethusdt", "adausdt", "avaxusdt"});
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Price Table
    priceTable = new QTableWidget(5, 2, this);
    priceTable->setHorizontalHeaderLabels({"Symbol", "Price"});
    priceTable->setItem(0, 0, new QTableWidgetItem("BTC/USDT"));
    priceTable->setItem(1, 0, new QTableWidgetItem("SOL/USDT"));
    priceTable->setItem(2, 0, new QTableWidgetItem("ETH/USDT"));
    priceTable->setItem(3, 0, new QTableWidgetItem("ADA/USDT"));
    priceTable->setItem(4, 0, new QTableWidgetItem("AVAX/USDT"));
    layout->addWidget(priceTable);

    // Positions Table
    positionsTable = new QTableWidget(0, 7, this);
    positionsTable->setHorizontalHeaderLabels({
        "Symbol", "Entry Price", "Current Price", "Amount", "PnL (USD)", "TP Price", "SL Price"
    });
    layout->addWidget(positionsTable);

    // Buttons
    QPushButton* addPositionButton = new QPushButton("Új pozíció", this);
    connect(addPositionButton, &QPushButton::clicked, this, &MainWindow::onAddPositionClicked);
    layout->addWidget(addPositionButton);

    QPushButton* closePositionButton = new QPushButton("Pozíció bezárása", this);
    connect(closePositionButton, &QPushButton::clicked, this, &MainWindow::onClosePositionClicked);
    layout->addWidget(closePositionButton);

    // Debug Console
    debugConsole = new QTextEdit(this);
    debugConsole->setReadOnly(true);
    layout->addWidget(debugConsole);

    setCentralWidget(centralWidget);
}

void MainWindow::updatePrice(const QString& symbol, double price) {
    for (int i = 0; i < priceTable->rowCount(); ++i) {
        if (priceTable->item(i, 0)->text() == symbol) {
            priceTable->item(i, 1)->setText(QString::number(price, 'f', 2));
            if (demoAccount->getPositions().contains(symbol)) {
                double pnl = demoAccount->calculatePnL(symbol, price);
                emit demoAccount->pnlUpdated(symbol, pnl);

                const Position& pos = demoAccount->getPositions()[symbol];
                if (price >= pos.tpPrice) emit demoAccount->tpReached(symbol);
                else if (price <= pos.slPrice) emit demoAccount->slReached(symbol);
            }
        }
    }
}

void MainWindow::onAddPositionClicked() {
    QDialog dialog(this);
    QFormLayout form(&dialog);
    QLineEdit symbolEdit, entryEdit, amountEdit, tpEdit, slEdit;
    form.addRow("Symbol:", &symbolEdit);
    form.addRow("Entry Price:", &entryEdit);
    form.addRow("Amount:", &amountEdit);
    form.addRow("TP Price:", &tpEdit);
    form.addRow("SL Price:", &slEdit);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
        demoAccount->addPosition(
            symbolEdit.text().toUpper(),
            entryEdit.text().toDouble(),
            amountEdit.text().toDouble(),
            tpEdit.text().toDouble(),
            slEdit.text().toDouble()
        );
        int row = positionsTable->rowCount();
        positionsTable->insertRow(row);
        positionsTable->setItem(row, 0, new QTableWidgetItem(symbolEdit.text().toUpper()));
        positionsTable->setItem(row, 1, new QTableWidgetItem(entryEdit.text()));
        positionsTable->setItem(row, 2, new QTableWidgetItem("0.0"));
        positionsTable->setItem(row, 3, new QTableWidgetItem(amountEdit.text()));
        positionsTable->setItem(row, 4, new QTableWidgetItem("0.0"));
        positionsTable->setItem(row, 5, new QTableWidgetItem(tpEdit.text()));
        positionsTable->setItem(row, 6, new QTableWidgetItem(slEdit.text()));
    });
    dialog.exec();
}

void MainWindow::onClosePositionClicked() {
    int row = positionsTable->currentRow();
    if (row >= 0) {
        QString symbol = positionsTable->item(row, 0)->text();
        demoAccount->closePosition(symbol);
        positionsTable->removeRow(row);
    }
}

