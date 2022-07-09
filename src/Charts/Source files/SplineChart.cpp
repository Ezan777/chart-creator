#include "../Header files/SplineChart.h"

SplineChart::SplineChart(SplineChartModel *chart_model, QWidget *parent): AbstractChart(chart_model, parent) {
    model = chart_model;

    points = new QList<QPointF>;
    AbstractChart::series = new QSplineSeries;

    dynamic_cast<QSplineSeries*>(AbstractChart::series)->append(*points);

    this->connectChart();

    setAnimationOptions(QChart::SeriesAnimations);

    addSeries(AbstractChart::series);
    createDefaultAxes();

    axisX = new QValueAxis;
    axisY = new QValueAxis;

    axes(Qt::Horizontal).replace(0, axisX);
    axes(Qt::Vertical).replace(0, axisY);

    legend()->hide();
}

SplineChart::~SplineChart() {
    delete points;
    delete axisX;
    delete axisY;
}

void SplineChart::loadFromModel() {
    points->clear();
    dynamic_cast<QSplineSeries*>(AbstractChart::series)->clear();
    for(int i = 0; i < model->getNumberOfContainersRows(); ++i) {
        points->append(QPointF(model->getLabelAt(i).toDouble(), model->getDataAt(i).toDouble()));
    }
    dynamic_cast<QSplineSeries*>(AbstractChart::series)->append(*points);
}

void SplineChart::updateAxesRange() {
    double min_data = model->getMinData(), max_data = model->getMaxData(), min_label = model->getMinLabel(), max_label = model->getMaxLabel();

    if(min_data >= 0) {
        axes(Qt::Vertical).at(0)->setRange(0, max_data + 0.10 * max_data);
    } else {
        axes(Qt::Vertical).at(0)->setRange(min_data + 0.10 * min_data, max_data + 0.10 * max_data);
    }

    if(min_label >= 0) {
        axes(Qt::Horizontal).at(0)->setRange(0, max_label + 0.10 * max_label);
    } else {
        axes(Qt::Horizontal).at(0)->setRange(min_label + 0.10 * min_label, max_label + 0.10 * max_label);
    }
}

void SplineChart::updateChartSets() {
    if(points->length() < model->getNumberOfContainersRows()) {
        //Point added
        points->append(QPointF(model->getLabelAt(model->getNumberOfContainersRows() - 1).toDouble(), model->getDataAt(model->getNumberOfContainersRows() - 1).toDouble()));
        dynamic_cast<QSplineSeries*>(AbstractChart::series)->replace(*points);
        updateChart();
    } else {
        if (points->length() == model->getNumberOfContainersRows()) {
            for (int i = 0; i < points->length(); ++i) {
                updateDataOnSetAt(i);
            }
        }
    }
}

void SplineChart::deleteSetAt(int index) {
    auto set_to_delete = points->at(index);
    points->removeAt(index);
    dynamic_cast<QSplineSeries*>(AbstractChart::series)->remove(set_to_delete);
}

void SplineChart::updateDataOnSetAt(int index) {
    points->replace(index, QPointF(model->getLabelAt(index).toDouble(), model->getDataAt(index).toDouble()));
    dynamic_cast<QSplineSeries*>(AbstractChart::series)->replace(*points);
    updateAxesRange();
}

void SplineChart::updateChart() {
    update();
    updateAxesRange();
}

void SplineChart::setColor(const QString& new_color) {
    dynamic_cast<QSplineSeries*>(AbstractChart::series)->setColor(new_color);
}