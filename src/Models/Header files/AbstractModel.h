#ifndef PROGETTOPAO_ABSTRACTMODEL_H
#define PROGETTOPAO_ABSTRACTMODEL_H

#include <QFile>
#include "QWidget"
#include "QXmlStreamWriter"
#include <QJsonArray>
#include "QJsonObject"

enum class ChartType { BarChart, PieChart, SplineChart };

class AbstractModel : public QWidget {
 Q_OBJECT
 protected:
  QList<QString> *labels;
  QList<QString> *colors;
  QList<QString> *data;
  QString chart_title;
  ChartType type;

  virtual void saveChartType(QXmlStreamWriter *stream) const = 0;
  void saveLabels(QXmlStreamWriter *stream) const;
  void saveColors(QXmlStreamWriter *stream) const;
  void saveData(QXmlStreamWriter *stream) const;
  void saveChartTitle(QXmlStreamWriter *stream) const;

  virtual void saveChartTypeToJson(QJsonObject *json) const = 0;
  void saveLabelsToJson(QJsonObject *json) const;
  void saveColorsToJson(QJsonObject *json) const;
  void saveDataToJson(QJsonObject *json) const;
  void saveChartTitleToJson(QJsonObject *json) const;

  virtual void loadChartType(QXmlStreamReader *stream);
  void loadLabels(QXmlStreamReader *stream);
  void loadColors(QXmlStreamReader *stream);
  void loadData(QXmlStreamReader *stream);
  void loadChartTitle(QXmlStreamReader *stream);

  virtual void loadChartTypeFromJson(const QJsonObject& json);
  void loadLabelsFromJson(const QJsonObject& json);
  void loadColorsFromJson(const QJsonObject& json);
  void loadDataFromJson(const QJsonObject& json);
  void loadChartTitleFromJson(const QJsonObject& json);
 public:
  AbstractModel();
  ~AbstractModel() override;
  virtual void clearModel();

  void removeLabelAt(int index);
  void removeDataAt(int index);

  double getMaxData() const;
  double getMinData() const;

  static ChartType getChartTypeFromXML(QFile *file);

  int getLabelsLength() const;

  void printLabels();
  void printData();
 public slots:
  QString getLabelAt(int index) const;
  virtual const QString &getColorAt(int index) const;
  QString getDataAt(int index) const;
  QString getChartTitle() const;
  int getNumberOfContainersRows() const;
  virtual ChartType getChartType() const = 0;

  int appendLabel(const QString &new_label);
  int appendColor(const QString &new_color);
  int appendData(const QString &new_data);
  void setChartTitle(const QString &title);

  void insertData(int index, const QString &new_data);

  void editExistingLabel(int index, const QString &new_label);
  void editExistingColor(int index, const QString &new_color);
  void editExistingData(int index, const QString &new_data);

  virtual void saveModelToXML(QXmlStreamWriter *stream) const;
  virtual void saveModelToJson(QJsonObject *json) const;

  virtual void loadModelFromXML(QXmlStreamReader *stream);
  virtual void loadModelFromJson(const QJsonObject& json);
 signals:
  void labelsChanged(int index);
  void dataRowDeleted(int index);
  void dataModifiedAt(int index);
  void updateChartSets();
  void deleteSetAt(int index);
  void updateChartColors();
  void updateColorsButtonsAt(int index);
  void titleChanged(const QString &new_title);
};

#endif //PROGETTOPAO_ABSTRACTMODEL_H
