#pragma once
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QFile>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    void saveConfig(const QString& telegramToken, const QString& chatId);
    void loadConfig(QString& telegramToken, QString& chatId);
};

