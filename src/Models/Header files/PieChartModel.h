#ifndef PROGETTOPAO_PIECHARTMODEL_H
#define PROGETTOPAO_PIECHARTMODEL_H

#include "AbstractModel.h"
#include "../../Charts/Header files/AbstractChart.h"

class PieChartModel : public AbstractModel {
 Q_OBJECT
 private:
  void saveChartType(QXmlStreamWriter *stream) const override;
  void saveChartTypeToJson(QJsonObject *json) const override;
 public:
  PieChartModel();
  ~PieChartModel() override;

  ChartType getChartType() const override;
 public slots:
  void saveModelToXML(QXmlStreamWriter *stream) const override;
  void saveModelToJson(QJsonObject *json) const override;

};

#endif //PROGETTOPAO_PIECHARTMODEL_H
