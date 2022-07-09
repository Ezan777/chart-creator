#ifndef PROGETTOPAO_PIECHART_H
#define PROGETTOPAO_PIECHART_H
#include "QtCharts/QChart"
#include "../../Models/Header files/BarChartModel.h"
#include "../../Charts/Header files/AbstractChart.h"
#include "QPieSlice"
#include "QPieSeries"
#include "../../Models/Header files/PieChartModel.h"
#include <cmath>


class PieChart: public AbstractChart {
    Q_OBJECT
private:
    QList<QtCharts::QPieSlice*> *slices;

    void connectChart() const override;
public:
    explicit PieChart(PieChartModel *chart_model, QWidget *parent = nullptr);
    ~PieChart() override;

    void loadFromModel() override;

    void darkMode() override;
    void lightMode() override;
public slots:
    void updateChartSets() override;
    void deleteSetAt(int index) override;
    void updateDataOnSetAt(int index) override;
    void updateColors();
    void updateChart() override;
};


#endif //PROGETTOPAO_PIECHART_H
