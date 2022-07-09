#include "../Header files/AbstractModel.h"
#include "QDebug"

AbstractModel::AbstractModel() {
  labels = new QList<QString>;
  colors = new QList<QString>;
  data = new QList<QString>;
  chart_title = "";
}

AbstractModel::~AbstractModel() {
  delete labels;
  delete colors;
  delete data;
}

void AbstractModel::clearModel() {
  labels->clear();
  colors->clear();
  data->clear();
}

void AbstractModel::removeLabelAt(int index) {
  labels->removeAt(index);
  if (!colors->isEmpty()) {
    colors->removeAt(index);
  }
  emit dataRowDeleted(index);
  emit updateChartSets();
  emit deleteSetAt(index);
  emit updateChartColors();
}

void AbstractModel::removeDataAt(int index) {
  data->removeAt(index);
}

double AbstractModel::getMaxData() const {
  if (data->length() != 0) {
    double max = data->first().toDouble();
    for (auto const &i : *data) {
      if (max < i.toDouble()) {
        max = i.toDouble();
      }
    }

    return max;
  } else {
    return 0;
  }
}

double AbstractModel::getMinData() const {
  if (data->length() != 0) {
    double min = data->first().toDouble();
    for (auto const &i : *data) {
      if (min > i.toDouble()) {
        min = i.toDouble();
      }
    }

    return min;
  } else {
    return 0;
  }
}

ChartType AbstractModel::getChartTypeFromXML(QFile *file) {
  QXmlStreamReader stream(file);

  while (stream.name() != "EndChartType") {
    if (stream.name() == "ChartType") {
      QString chart_type = stream.readElementText();

      if (chart_type == "PieChart") {
        return ChartType::PieChart;
      } else {
        if (chart_type == "BarChart") {
          return ChartType::BarChart;
        } else {
          if (chart_type == "SplineChart") {
            return ChartType::SplineChart;
          }
        }
      }
    }
    stream.readNext();
  }
}

QString AbstractModel::getLabelAt(int index) const {
  return labels->at(index);
}

const QString &AbstractModel::getColorAt(int index) const {
  return colors->at(index);
}

QString AbstractModel::getDataAt(int index) const {
  return data->at(index);
}

QString AbstractModel::getChartTitle() const {
  return chart_title;
}

/// Returns the number of labels
int AbstractModel::getNumberOfContainersRows() const {
  return labels->length();
}

int AbstractModel::appendLabel(const QString &new_label) {
  labels->append(new_label);
  //emit updateChartSets();
  return labels->length() - 1;
}

int AbstractModel::appendColor(const QString &new_color) {
  colors->append(new_color);
  emit updateChartColors();
  return colors->length() - 1;
}

int AbstractModel::appendData(const QString &new_data) {
  data->append(new_data);
  emit updateChartSets();
  return data->length() - 1;
}

void AbstractModel::setChartTitle(const QString &title) {
  chart_title = title;
  emit titleChanged(chart_title);
}

void AbstractModel::insertData(int index, const QString &new_data) {
  data->insert(index, new_data);
  emit updateChartSets();
}

void AbstractModel::editExistingLabel(int index, const QString &new_label) {
  labels->replace(index, new_label);
  emit labelsChanged(index);
  emit updateChartSets();
  emit updateChartColors();
}

void AbstractModel::editExistingColor(int index, const QString &new_color) {
  colors->replace(index, new_color);
  emit updateColorsButtonsAt(index);
  emit updateChartColors();
}

void AbstractModel::editExistingData(int index, const QString &new_data) {
  data->replace(index, new_data);
  emit dataModifiedAt(index);
}

void AbstractModel::saveLabels(QXmlStreamWriter *stream) const {
  for (const auto &label : *labels) {
    stream->writeTextElement("Label", label);
  }
  stream->writeTextElement("EndOfLabels", "end of labels");

}

void AbstractModel::saveColors(QXmlStreamWriter *stream) const {
  for (const auto &color : *colors) {
    stream->writeTextElement("Color", color);
  }
  stream->writeTextElement("EndOfColors", "end of colors");

}

void AbstractModel::saveData(QXmlStreamWriter *stream) const {
  for (auto const &d : *data) {
    stream->writeTextElement("Data", d);
  }
  stream->writeTextElement("EndOfData", "end of data");

}

void AbstractModel::saveChartTitle(QXmlStreamWriter *stream) const {
  stream->writeTextElement("ChartTitle", chart_title);
  stream->writeTextElement("EndOfChartTitle", "end of chart title");
}

void AbstractModel::saveLabelsToJson(QJsonObject *json) const {
  QJsonArray labels_array;
  for (const auto &label : *labels) {
    labels_array.append(label);
  }
  (*json)["labels"] = labels_array;
}

void AbstractModel::saveColorsToJson(QJsonObject *json) const {
  QJsonArray colors_array;
  for (const auto &color : *colors) {
    colors_array.append(color);
  }
  (*json)["colors"] = colors_array;
}

void AbstractModel::saveDataToJson(QJsonObject *json) const {
  QJsonArray data_array;
  for (const auto &number : *data) {
    data_array.append(number);
  }
  (*json)["data"] = data_array;
}

void AbstractModel::saveChartTitleToJson(QJsonObject *json) const {
  (*json)["chartTitle"] = chart_title;
}

void AbstractModel::loadChartType(QXmlStreamReader *stream) {
  while (stream->name() != "EndChartType") {
    if (stream->name() == "ChartType") {
      QString chart_type = stream->readElementText();

      if (chart_type == "PieChart") {
        type = ChartType::PieChart;
      } else {
        if (chart_type == "BarChart") {
          type = ChartType::BarChart;
        }
      }
    }
    stream->readNext();
  }
}

void AbstractModel::loadLabels(QXmlStreamReader *stream) {
  while (stream->name() != "EndOfLabels") {
    if (stream->name() == "Label") {
      labels->append(stream->readElementText());
    }
    stream->readNext();
  }
}

void AbstractModel::loadColors(QXmlStreamReader *stream) {
  while (stream->name() != "EndOfColors") {
    if (stream->name() == "Color") {
      appendColor(stream->readElementText());
    }
    stream->readNext();
  }
}

void AbstractModel::loadData(QXmlStreamReader *stream) {
  while (stream->name() != "EndOfData") {
    if (stream->name() == "Data") {
      data->append((stream->readElementText()));
    }

    stream->readNext();
  }
  emit updateChartSets();
}

void AbstractModel::loadChartTitle(QXmlStreamReader *stream) {
  while (stream->name() != "EndOfChartTitle") {
    if (stream->name() == "ChartTitle") {
      chart_title = stream->readElementText();
    }
    stream->readNext();
  }
}

void AbstractModel::loadChartTypeFromJson(const QJsonObject &json) {
  QString chart_type = json["chartType"].toString();
  if(chart_type == "BarChart") {
    type = ChartType::BarChart;
  } else {
    if(chart_type == "PieChart") {
      type = ChartType::PieChart;
    } else {
      if(chart_type == "SplineChart") {
        type = ChartType::SplineChart;
      }
    }
  }
}

void AbstractModel::loadLabelsFromJson(const QJsonObject &json) {
  QJsonArray labels_array = json["labels"].toArray();
  for(const auto& label: labels_array) {
    labels->append(label.toString());
  }
}

void AbstractModel::loadColorsFromJson(const QJsonObject &json) {
  QJsonArray colors_array = json["colors"].toArray();
  for(const auto& color: colors_array) {
    colors->append(color.toString());
  }
}

void AbstractModel::loadDataFromJson(const QJsonObject &json) {
  QJsonArray data_array = json["data"].toArray();
  for(const auto& number: data_array) {
    data->append(number.toString());
  }
}

void AbstractModel::loadChartTitleFromJson(const QJsonObject &json) {
  chart_title = json["chartTitle"].toString();
}

void AbstractModel::saveModelToXML(QXmlStreamWriter *stream) const {
  saveLabels(stream);
  saveColors(stream);
  saveData(stream);
  saveChartTitle(stream);
}

void AbstractModel::saveModelToJson(QJsonObject *json) const {
  saveLabelsToJson(json);
  saveColorsToJson(json);
  saveDataToJson(json);
  saveChartTitleToJson(json);
}

void AbstractModel::loadModelFromXML(QXmlStreamReader *stream) {
  loadChartType(stream);
  loadLabels(stream);
  loadColors(stream);
  loadData(stream);
  loadChartTitle(stream);
}

void AbstractModel::loadModelFromJson(const QJsonObject &json) {
  loadChartTypeFromJson(json);
  loadLabelsFromJson(json);
  loadColorsFromJson(json);
  loadDataFromJson(json);
  loadChartTitleFromJson(json);
}

int AbstractModel::getLabelsLength() const {
  return labels->length();
}

void AbstractModel::printLabels() {
  for (auto const &label : *labels) {
    qDebug() << "Label: " << label;
  }
}

void AbstractModel::printData() {
  for (auto const &d : *data) {
    qDebug() << "Data: " << d;
  }
}
