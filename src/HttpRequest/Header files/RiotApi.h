//
// Created by enrico on 20/06/22.
//

#ifndef PROGETTOPAO_SRC_HTTPREQUEST_HEADER_FILES_RIOTAPI_H_
#define PROGETTOPAO_SRC_HTTPREQUEST_HEADER_FILES_RIOTAPI_H_
#include "QWidget"
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkRequest"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QtNetwork/QNetworkReply"
#include "QEventLoop"

class RiotApi: public QWidget {
  Q_OBJECT
 private:
  static QNetworkAccessManager manager;
  static QString api_key;

  static QString * puuid(const QString& summoner_name);
  static QString * lastRanked(QString *summoner_puuid);
 public:
  static QJsonObject* lastMatchInfo(const QString& summoner_name);
};

#endif //PROGETTOPAO_SRC_HTTPREQUEST_HEADER_FILES_RIOTAPI_H_
