#ifndef PROGETTOPAO_ABSTRACTCHART_H
#define PROGETTOPAO_ABSTRACTCHART_H
#include "QtCharts/QChart"
#include "../../Models/Header files/AbstractModel.h"

using namespace QtCharts;

enum class ChartType;

class AbstractChart: public QChart {
    Q_OBJECT
protected:
    AbstractModel *model;
    QAbstractSeries *series;

    virtual void connectChart() const;
public:
    AbstractChart(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~AbstractChart() override;

    virtual void loadFromModel() = 0;

    virtual void darkMode();
    virtual void lightMode();
public slots:
    virtual void updateChartSets() = 0;
    virtual void deleteSetAt(int index) = 0;
    virtual void updateDataOnSetAt(int index) = 0;
    virtual void updateChart() = 0;
};


#endif //PROGETTOPAO_ABSTRACTCHART_H
