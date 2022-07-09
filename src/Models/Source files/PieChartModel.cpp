#include "../Header files/PieChartModel.h"
#include "QDebug"

PieChartModel::PieChartModel() {
  type = ChartType::PieChart;
}

PieChartModel::~PieChartModel() = default;

void PieChartModel::saveChartType(QXmlStreamWriter *stream) const {
  stream->writeTextElement("ChartType", "PieChart");
  stream->writeTextElement("EndChartType", "end of type");
}

void PieChartModel::saveChartTypeToJson(QJsonObject *json) const {
  (*json)["chartType"] = "PieChart";
}

ChartType PieChartModel::getChartType() const {
  return type;
}

void PieChartModel::saveModelToXML(QXmlStreamWriter *stream) const {
  saveChartType(stream);
  AbstractModel::saveModelToXML(stream);
}

void PieChartModel::saveModelToJson(QJsonObject *json) const {
  saveChartTypeToJson(json);
  AbstractModel::saveModelToJson(json);
}