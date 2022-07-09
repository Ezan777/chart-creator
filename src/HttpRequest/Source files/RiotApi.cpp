//
// Created by enrico on 20/06/22.
//

#include <QException>
#include "../Header files/RiotApi.h"
#include "../Header files/ApiKey.h"
QNetworkAccessManager RiotApi::manager;
QString RiotApi::api_key = API_KEY;

QString * RiotApi::puuid(const QString &summoner_name) {
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(
      "https://euw1.api.riotgames.com/lol/summoner/v4/summoners/by-name/"
          +summoner_name+"?api_key="+api_key
  )));
  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if(reply->isFinished()) {
    if(reply->error() != QNetworkReply::NetworkError::NoError) {
      if(reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
        throw QString("Summoner name not found");
      } else {
        throw QString("Error with the http request" + reply->errorString());
      }
    } else {
      QByteArray data = reply->readAll();
      QJsonDocument json_document(QJsonDocument::fromJson(data));
      return new QString(json_document.object()["puuid"].toString());
    }
  }
}

QString * RiotApi::lastRanked(QString *summoner_puuid) {
  QNetworkReply *reply;
  try{
    reply = manager.get(QNetworkRequest(QUrl(
        "https://europe.api.riotgames.com/lol/match/v5/matches/by-puuid/"
            +*summoner_puuid+"/ids?type=ranked&start=0&count=20&api_key="+api_key
    )));
  } catch (const QString& error_string) {
    throw error_string;
  }

  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if(reply->isFinished()) {
    if(reply->error() != QNetworkReply::NetworkError::NoError) {
      if(reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
        throw QString("Ranked not found");
      } else {
        throw QString("Error with the http request" + reply->errorString());
      }
    } else {
      QByteArray data = reply->readAll();
      QJsonDocument json_document(QJsonDocument::fromJson(data));
      return new QString(json_document.array()[0].toString());
    }
  }
}

QJsonObject* RiotApi::lastMatchInfo(const QString &summoner_name) {
  QString *summoner_puuid = puuid(summoner_name);
  QString *match_code = lastRanked(summoner_puuid);

  QNetworkReply *reply;

  try{
    reply = manager.get(QNetworkRequest(QUrl(
        "https://europe.api.riotgames.com/lol/match/v5/matches/"
            +*match_code+"?api_key="+api_key
    )));
  } catch (const QString& error_string) {
    throw error_string;
  }

  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if(reply->isFinished()) {
    if(reply->error() != QNetworkReply::NetworkError::NoError) {
      if(reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
        throw QString("Summoner name not found");
      } else {
        throw QString("Error with the http request" + reply->errorString());
      }
    } else {
      QByteArray data = reply->readAll();
      QJsonDocument json_document(QJsonDocument::fromJson(data));
      delete summoner_puuid;
      delete match_code;
      return new QJsonObject(json_document.object());
    }
  }
}