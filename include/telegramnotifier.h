#pragma once
#include <QObject>
#include <QString>

class TelegramNotifier : public QObject {
    Q_OBJECT
public:
    TelegramNotifier(const QString& token, const QString& chatId, QObject* parent = nullptr);
    void sendMessage(const QString& message);

private:
    QString token;
    QString chatId;
};
