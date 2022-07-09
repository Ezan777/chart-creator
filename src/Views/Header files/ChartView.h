#ifndef PROGETTOPAO_CHARTVIEW_H
#define PROGETTOPAO_CHARTVIEW_H
#include "QWidget"
#include "QtCharts/QChart"
#include "QtCharts/QChartView"
#include "QtCharts/QBarLegendMarker"
#include "QtCharts/QBarCategoryAxis"
#include "QtCharts/QBarSeries"
#include "QtCharts/QBarSet"
#include "QVBoxLayout"
#include "../../Models/Header files/BarChartModel.h"
#include <cmath>
#include "QFileDialog"
#include "QStandardPaths"
#include "../../Charts/Header files/AbstractChart.h"
#include "../../Charts/Header files/BarChart.h"
#include "../../Charts/Header files/PieChart.h"
#include "QComboBox"
#include "../../Charts/Header files/SplineChart.h"
#include "../../Buttons/Header files/SelectColorButton.h"

class ChartView : public QWidget {
 Q_OBJECT
 private:
  AbstractModel *model;
  QFont *title_font;
  QHBoxLayout *title_layout;
  QLabel *title_head;
  QLineEdit *title;
  QVBoxLayout *layout;
  QtCharts::QChartView *chart_view;
  AbstractChart *chart;
 public:
  explicit ChartView(AbstractModel *chart_model, QWidget *parent = nullptr);
  ~ChartView() override;
  void darkMode();
  void lightMode();
 public slots:
  void updateChartView();
  void saveChartAsImage();
  void changeChartTitle(const QString &new_title);
};

#endif //PROGETTOPAO_CHARTVIEW_H
