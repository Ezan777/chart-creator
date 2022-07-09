#ifndef PROGETTOPAO_BARCHARTMODEL_H
#define PROGETTOPAO_BARCHARTMODEL_H

#include <QWidget>
#include <QXmlStreamWriter>
#include "QDebug"
#include "AbstractModel.h"
#include "../../Charts/Header files/AbstractChart.h"

class BarChartModel : public AbstractModel {
 Q_OBJECT
 private:
  QList<QString> *series;
  ChartType type;

  void saveChartType(QXmlStreamWriter *stream) const override;
  void saveChartTypeToJson(QJsonObject *json) const override;

  void saveSeries(QXmlStreamWriter *stream) const;
  void saveSeriesToJson(QJsonObject *json) const;

  void loadSeries(QXmlStreamReader *stream);
  void loadSeriesFromJson(const QJsonObject &json);

  void saveRowsForContainer(QXmlStreamWriter *stream) const;

 public:
  BarChartModel();
  ~BarChartModel() override;
  void clearModel() override;

  void removeSeriesAt(int index);
 public slots:
  QString getSeriesAt(int index) const;
  QStringList getAllSeries() const;
  int getSeriesNumber() const;
  ChartType getChartType() const override;

  int appendSeries(const QString &new_series);

  void editExistingSeries(int index, const QString &new_series);

  void saveModelToXML(QXmlStreamWriter *stream) const override;
  void saveModelToJson(QJsonObject *json) const override;

  void loadModelFromXML(QXmlStreamReader *stream) override;
  void loadModelFromJson(const QJsonObject &json) override;
 signals:
  void seriesChanged();
  void seriesRowDeleted(int index);
  void addedNewSeries();
  void seriesTextChanged();
};

#endif //PROGETTOPAO_BARCHARTMODEL_H
