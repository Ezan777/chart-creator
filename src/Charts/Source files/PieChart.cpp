#include <QChartView>
#include "../Header files/PieChart.h"
#include "../../Models/Header files/PieChartModel.h"

PieChart::PieChart(PieChartModel *chart_model, QWidget *parent): AbstractChart(chart_model, parent) {
    setParent(parent);
    
    slices = new QList<QtCharts::QPieSlice*>;
    AbstractChart::series = new QtCharts::QPieSeries(this);

    dynamic_cast<QtCharts::QPieSeries*>(AbstractChart::series)->append(*slices);

    this->connectChart();

    setAnimationOptions(QChart::SeriesAnimations);

    addSeries(series);
}

PieChart::~PieChart() {
    /*for(auto slice: *slices) {
        delete slice;
    }*/
    delete slices;
}

void PieChart::loadFromModel() {
    slices->clear();
    dynamic_cast<QPieSeries*>(AbstractChart::series)->clear();
    for(int i = 0; i < model->getNumberOfContainersRows(); ++i) {
        slices->append(new QPieSlice(model->getLabelAt(i), model->getDataAt(i).toDouble()));
        dynamic_cast<QPieSeries*>(AbstractChart::series)->append(slices->last());
    }
    updateColors();
}

void PieChart::connectChart() const {
    AbstractChart::connectChart();
    if (model) {
        connect(model, &AbstractModel::updateChartColors, this, &PieChart::updateColors);
    }
}

void PieChart::darkMode() {
    AbstractChart::darkMode();
    updateColors();
}

void PieChart::lightMode() {
    AbstractChart::lightMode();
    updateColors();
}

void PieChart::updateChartSets() {
    auto pie_series = dynamic_cast<QPieSeries*>(AbstractChart::series);
    
    if (pie_series) {
        //Check labels
        if (slices->length() < model->getNumberOfContainersRows()) {
            //New label has been added
            slices->append(new QPieSlice(model->getLabelAt(model->getNumberOfContainersRows() - 1),
                                         0));
            slices->last()->setLabelVisible(true);
            pie_series->append(slices->last());
            updateChart();
        } else {
            if (slices->length() == model->getNumberOfContainersRows()) {
                //Same number, it means I changed one label text
                for (int i = 0; i < slices->length(); ++i) {
                    if (slices->at(i)->label() != model->getLabelAt(i)) {
                        slices->at(i)->setLabel(model->getLabelAt(i));
                    }
                }
            }
        }
    }
}

void PieChart::deleteSetAt(int index) {
    auto set_to_delete = slices->at(index);
    slices->removeAt(index);
    dynamic_cast<QPieSeries*>(AbstractChart::series)->remove(set_to_delete);
}

void PieChart::updateDataOnSetAt(int index) {
    slices->at(index)->setValue(model->getDataAt(index).toDouble());
}

void PieChart::updateColors() {
    for(int i = 0; i < slices->length(); ++i) {
        slices->at(i)->setColor(model->getColorAt(i));
    }
}

void PieChart::updateChart() {
    update();
    updateChartSets();
}