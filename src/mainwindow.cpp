#include "mainwindow.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

// ha van ConfigManager::load(), meghívjuk; ha nincs, nem baj
#include <concepts>
template <class T>
concept HasLoad = requires(T t) { { t.load() }; };
template <class T>
static inline void maybe_call_load(T& t) {
    if constexpr (HasLoad<T>) t.load();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // cfg_.load();  // -> helyette:
    maybe_call_load(cfg_);

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

        // 5 paraméteres addPosition-hoz igazítva
        demo_.addPosition(sym, entry, amount, tp, sl);
        qDebug() << "[Added]" << sym << entry << amount << tp << sl;
    });
}
