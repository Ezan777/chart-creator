#ifndef PROGETTOPAO_SPLINECHART_H
#define PROGETTOPAO_SPLINECHART_H

#include "AbstractChart.h"
#include "QtCharts/QSplineSeries"
#include "QtCharts"
#include "../../Models/Header files/SplineChartModel.h"

using namespace QtCharts;

class SplineChart: public AbstractChart {
    Q_OBJECT
private:
    SplineChartModel *model;
    QList<QPointF> *points;
    QValueAxis *axisX, *axisY;
public:
    explicit SplineChart(SplineChartModel *chart_model, QWidget *parent = nullptr);
    ~SplineChart() override;

    void loadFromModel() override;

    void updateAxesRange();
public slots:
    void updateChartSets() override;
    void deleteSetAt(int index) override;
    void updateDataOnSetAt(int index) override;
    void updateChart() override;
    void setColor(const QString& new_color);
};


#endif //PROGETTOPAO_SPLINECHART_H
