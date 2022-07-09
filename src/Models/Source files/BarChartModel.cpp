#include "../Header files/BarChartModel.h"

BarChartModel::BarChartModel() {
  series = new QList<QString>;
  type = ChartType::BarChart;
}

BarChartModel::~BarChartModel() {
  delete series;
}

void BarChartModel::saveChartType(QXmlStreamWriter *stream) const {
  stream->writeTextElement("ChartType", "BarChart");
  stream->writeTextElement("EndChartType", "end of type");
}

void BarChartModel::saveChartTypeToJson(QJsonObject *json) const {
  (*json)["chartType"] = "BarChart";
}

void BarChartModel::clearModel() {
  labels->clear();
  data->clear();
  series->clear();
}

void BarChartModel::removeSeriesAt(int index) {
  series->removeAt(index);
  emit seriesChanged();
  emit seriesRowDeleted(index);
}

QString BarChartModel::getSeriesAt(int index) const {
  return series->at(index);
}

QStringList BarChartModel::getAllSeries() const {
  return {*series};
}

int BarChartModel::getSeriesNumber() const {
  return series->length();
}

ChartType BarChartModel::getChartType() const {
  return type;
}

int BarChartModel::appendSeries(const QString &new_series) {
  series->append(new_series);
  emit seriesChanged();
  for (int i = 0; i < labels->length(); ++i) {
    emit labelsChanged(i);
  }
  emit addedNewSeries();
  return series->length() - 1;
}

void BarChartModel::editExistingSeries(int index, const QString &new_series) {
  series->replace(index, new_series);
  emit seriesChanged();
  emit seriesTextChanged();
}

void BarChartModel::saveSeries(QXmlStreamWriter *stream) const {
  for (const auto &serie : *series) {
    stream->writeTextElement("Serie", serie);
  }
  stream->writeTextElement("SeriesNumber", QString(QString::number(series->length())));
  stream->writeTextElement("SeriesEnd", "end of series");
}

void BarChartModel::saveSeriesToJson(QJsonObject *json) const {
  QJsonArray series_array;
  for (const auto &serie : *series) {
    series_array.append(serie);
  }
  (*json)["series"] = series_array;
}

void BarChartModel::saveRowsForContainer(QXmlStreamWriter *stream) const {
  if (series->length() == 0) {
    stream->writeTextElement("NumberOfRows", "0");
  } else {
    stream->writeTextElement("NumberOfRows", QString::number(labels->length()));
  }
  stream->writeTextElement("ContainersEnd", "end of containers");
}

void BarChartModel::loadSeries(QXmlStreamReader *stream) {
  while (stream->name() != "SeriesEnd") {
    if (stream->name() == "Serie") {
      series->append(stream->readElementText());
    }
    stream->readNext();
  }
}

void BarChartModel::loadSeriesFromJson(const QJsonObject &json) {
  QJsonArray series_array = json["series"].toArray();
  for (const auto &serie : series_array) {
    series->append(serie.toString());
  }
}

void BarChartModel::saveModelToXML(QXmlStreamWriter *stream) const {
  saveSeries(stream);
  saveRowsForContainer(stream);
  saveChartType(stream);
  AbstractModel::saveModelToXML(stream);
}

void BarChartModel::saveModelToJson(QJsonObject *json) const {
  saveChartTypeToJson(json);
  saveSeriesToJson(json);
  AbstractModel::saveModelToJson(json);
}

void BarChartModel::loadModelFromXML(QXmlStreamReader *stream) {
  loadSeries(stream);
  AbstractModel::loadModelFromXML(stream);
}

void BarChartModel::loadModelFromJson(const QJsonObject &json) {
  loadSeriesFromJson(json);
  AbstractModel::loadModelFromJson(json);
}