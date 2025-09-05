#include "telegramnotifier.h"
#include <curl/curl.h>
#include <QDebug>

TelegramNotifier::TelegramNotifier(const QString& token, const QString& chatId, QObject* parent)
    : QObject(parent), token(token), chatId(chatId) {}

void TelegramNotifier::sendMessage(const QString& message) {
    CURL* curl = curl_easy_init();
    if (curl) {
        QString url = QString("https://api.telegram.org/bot%1/sendMessage?chat_id=%2&text=%3")
                          .arg(token, chatId, message);
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            qDebug() << "Telegram error: " << curl_easy_strerror(res);
        }
        curl_easy_cleanup(curl);
    }
}
