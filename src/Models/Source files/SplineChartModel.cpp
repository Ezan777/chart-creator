#include "../Header files/SplineChartModel.h"

SplineChartModel::SplineChartModel() {
  type = ChartType::SplineChart;
}

SplineChartModel::~SplineChartModel() = default;

void SplineChartModel::saveChartType(QXmlStreamWriter *stream) const {
  stream->writeTextElement("ChartType", "SplineChart");
  stream->writeTextElement("EndChartType", "end of chart type");
}

void SplineChartModel::saveChartTypeToJson(QJsonObject *json) const {
  (*json)["chartType"] = "SplineChart";
}

ChartType SplineChartModel::getChartType() const {
  return type;
}

bool SplineChartModel::areThereColors() const {
  return !AbstractModel::colors->isEmpty();
}

double SplineChartModel::getMaxLabel() const {
  if (labels->length() != 0) {
    double max = labels->first().toDouble();
    for (auto const &label : *labels) {
      if (max < label.toDouble()) {
        max = label.toDouble();
      }
    }

    return max;
  } else {
    return 0;
  }
}

double SplineChartModel::getMinLabel() const {
  if (labels->length() != 0) {
    double min = labels->first().toDouble();
    for (auto const &label : *labels) {
      if (min > label.toDouble()) {
        min = label.toDouble();
      }
    }

    return min;
  } else {
    return 0;
  }
}

void SplineChartModel::saveModelToXML(QXmlStreamWriter *stream) const {
  saveChartType(stream);
  AbstractModel::saveModelToXML(stream);
}

void SplineChartModel::saveModelToJson(QJsonObject *json) const {
  saveChartTypeToJson(json);
  AbstractModel::saveModelToJson(json);
}