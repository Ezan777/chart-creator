#ifndef PROGETTOPAO_SPLINECHARTMODEL_H
#define PROGETTOPAO_SPLINECHARTMODEL_H

#include "AbstractModel.h"
#include <random>

class SplineChartModel : public AbstractModel {
 private:
  void saveChartType(QXmlStreamWriter *stream) const override;
  void saveChartTypeToJson(QJsonObject *json) const override;
 public:
  SplineChartModel();
  ~SplineChartModel() override;

  ChartType getChartType() const override;

  bool areThereColors() const;

  double getMaxLabel() const;
  double getMinLabel() const;
 public slots:
  void saveModelToXML(QXmlStreamWriter *stream) const override;
  void saveModelToJson(QJsonObject *json) const override;
};

#endif //PROGETTOPAO_SPLINECHARTMODEL_H
