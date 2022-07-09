#ifndef PROGETTOPAO_BARCHART_H
#define PROGETTOPAO_BARCHART_H

#include <QBarSet>
#include <QBarCategoryAxis>
#include "AbstractChart.h"
#include "QtCharts"
#include "../../Models/Header files/BarChartModel.h"

using namespace QtCharts;


class BarChart: public AbstractChart {
    Q_OBJECT
private:
    QList<QBarSet*> *bar_sets;
    QAbstractBarSeries *series;

    void connectChart() const override;
protected:
    QtCharts::QBarCategoryAxis *category_axis;
public:
    explicit BarChart(BarChartModel *chart_model, QWidget *parent = nullptr);
    ~BarChart() override;

    void updateAxesYRange();

    void loadFromModel() override;

    void darkMode() override;
    void lightMode() override;
public slots:
    void updateChartSets() override;
    void deleteSetAt(int index) override;
    void updateDataOnSetAt(int index) override;

    void initializeNewCategory();
    void removeCategory(int container_index);

    void updateCategories();
    void updateColors();
    void updateChart() override;
};


#endif //PROGETTOPAO_BARCHART_H
