#include "mainwindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    cfg_.load(); // ha máshogy hívod, cseréld erre az egy sorra

    buildUi();

    // példa streamek
    ws_.connectToBinance(QStringList{"BTCUSDT","ETHUSDT"});
}

void MainWindow::buildUi() {
    auto *central    = new QWidget(this);
    auto *form       = new QFormLayout();
    auto *symbolEdit = new QLineEdit(central);
    auto *entryEdit  = new QLineEdit(central);
    auto *amountEdit = new QLineEdit(central);
    auto *tpEdit     = new QLineEdit(central);
    auto *slEdit     = new QLineEdit(central);

    form->addRow("Symbol",        symbolEdit);
    form->addRow("Entry Price",   entryEdit);
    form->addRow("Amount",        amountEdit);
    form->addRow("Take Profit",   tpEdit);
    form->addRow("Stop Loss",     slEdit);

    auto *btnAdd = new QPushButton("Add position", central);

    auto *vbox = new QVBoxLayout();
    vbox->addLayout(form);
    vbox->addWidget(btnAdd);
    central->setLayout(vbox);
    setCentralWidget(central);

    connect(btnAdd, &QPushButton::clicked, this,
            [this, symbolEdit, entryEdit, amountEdit, tpEdit, slEdit]{
        const QString sym = symbolEdit->text().trimmed().toUpper();

        bool ok1=false, ok2=false, ok3=false, ok4=false;
        const double entry  = entryEdit->text().toDouble(&ok1);
        const double amount = amountEdit->text().toDouble(&ok2);
        const double tp     = tpEdit->text().toDouble(&ok3);
        const double sl     = slEdit->text().toDouble(&ok4);
        if(!(ok1 && ok2 && ok3 && ok4)) {
            qDebug() << "Invalid numbers";
            return;
        }

        // A te headeredhez igazítva – ez az 5 paraméteres verzió:
        // void addPosition(const QString&, double, double, double, double);
        demo_.addPosition(sym, entry, amount, tp, sl);
        qDebug() << "[Added]" << sym << entry << amount << tp << sl;
    });
}
