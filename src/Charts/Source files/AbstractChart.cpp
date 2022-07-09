#include "../Header files/AbstractChart.h"

AbstractChart::AbstractChart(AbstractModel *chart_model, QWidget *parent) {
    setParent(parent);

    model = chart_model;
    series = nullptr;
}

AbstractChart::~AbstractChart() = default;

void AbstractChart::connectChart() const {
    if (model) {
        connect(model, &AbstractModel::updateChartSets, this, &AbstractChart::updateChartSets);
        connect(model, &AbstractModel::dataModifiedAt, this, &AbstractChart::updateDataOnSetAt);
        connect(model, &AbstractModel::deleteSetAt, this, &AbstractChart::deleteSetAt);
    }
}

void AbstractChart::darkMode() {
    setTheme(QChart::ChartThemeDark);
}

void AbstractChart::lightMode() {
    setTheme(QChart::ChartThemeLight);
}