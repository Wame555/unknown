#pragma once
#include <QMainWindow>
#include "configmanager.h"
#include "demoaccount.h"
#include "binancewebsocket.h"

class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void buildUi();

    ConfigManager     cfg_;   // default ctor
    DemoAccount       demo_;
    BinanceWebsocket  ws_;
};
