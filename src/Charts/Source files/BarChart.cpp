#include "../Header files/BarChart.h"

BarChart::BarChart(BarChartModel *chart_model, QWidget *parent): AbstractChart(chart_model, parent) {
    bar_sets = new QList<QBarSet*>;
    category_axis = new QBarCategoryAxis;
    //Polymorphism
    series = new QBarSeries;

    addSeries(series);
    
    createDefaultAxes();

    setAnimationOptions(QChart::SeriesAnimations);

    this->connectChart();
}

BarChart::~BarChart() {
    for(auto bar_set : *bar_sets) {
        delete bar_set;
    }
    delete bar_sets;
    //delete category_axis;
}

void BarChart::connectChart() const {
    auto bar_model = dynamic_cast<BarChartModel*>(model);
    AbstractChart::connectChart();
    if (bar_model) {
        connect(bar_model, &BarChartModel::seriesRowDeleted, this, &BarChart::removeCategory);
        connect(bar_model, &BarChartModel::addedNewSeries, this, &BarChart::initializeNewCategory);
        connect(bar_model, &BarChartModel::seriesTextChanged, this, &BarChart::updateCategories);
        connect(bar_model, &BarChartModel::updateChartColors, this, &BarChart::updateColors);
    }
}

void BarChart::updateAxesYRange() {
    double min = model->getMinData(), max = model->getMaxData();
    if(min >= 0) {
        axes(Qt::Vertical).at(0)->setRange(0, max + 0.15 * max);
    } else {
        axes(Qt::Vertical).at(0)->setRange(min + 0.15 * min, max + 0.15 * max);
    }
}

void BarChart::loadFromModel() {
    bar_sets->clear();
    series->clear();
    for(int i = 0; i < model->getNumberOfContainersRows(); ++i) {
        bar_sets->append(new QBarSet(model->getLabelAt(i)));
        series->append(bar_sets->last());
        for(int j = 0; j < dynamic_cast<BarChartModel*>(model)->getSeriesNumber(); ++j) {
          bar_sets->last()->append(model->getDataAt(i + model->getNumberOfContainersRows() * j).toDouble());
        }
    }
    createDefaultAxes();
    updateCategories();
    updateColors();
}

void BarChart::darkMode() {
    AbstractChart::darkMode();
    updateColors();
}

void BarChart::lightMode() {
    AbstractChart::lightMode();
    updateColors();
}

void BarChart::updateChartSets() {
    auto bar_model = dynamic_cast<BarChartModel*>(model);
    //auto bar_series = dynamic_cast<QBarSeries*>(series);
    if (series) {
        //Check labels
        if (bar_sets->length() < bar_model->getNumberOfContainersRows()) {
            //New label added
            bar_sets->append(new QtCharts::QBarSet(bar_model->getLabelAt(bar_model->getNumberOfContainersRows() - 1)));
            for (int i = 0; i < bar_model->getSeriesNumber(); ++i) {
                //Populating the bar set with 0s for every category
                bar_sets->last()->append(0);
            }
            series->append(bar_sets->last());
        } else {
            if(bar_sets->length() == model->getNumberOfContainersRows()) {
                //Same number, it means I changed one label text
                for (int i = 0; i < bar_sets->length(); ++i) {
                    if (bar_sets->at(i)->label() != bar_model->getLabelAt(i)) {
                        bar_sets->at(i)->setLabel(bar_model->getLabelAt(i));
                    }
                }
            }
        }
    }
}

void BarChart::deleteSetAt(int index) {
    auto set_to_delete = bar_sets->at(index);
    bar_sets->removeAt(index);
    series->remove(set_to_delete);
}

void BarChart::updateDataOnSetAt(int index) {
    int container_index, label_index;
    if (model->getNumberOfContainersRows() != 0) {
        container_index = floor(index / model->getNumberOfContainersRows());
        label_index = index - model->getNumberOfContainersRows() * container_index;
    } else {
        container_index = 0;
        label_index = 0;
    }

    bar_sets->at(label_index)->replace(container_index, model->getDataAt(index).toDouble());
    updateAxesYRange();
}

void BarChart::initializeNewCategory() {
    for(auto bar_set : *bar_sets) {
        bar_set->append(0);
    }
    updateCategories();
    updateChartSets();
}

void BarChart::removeCategory(int container_index) {
    for(auto bar_set : *bar_sets) {
        bar_set->remove(container_index);
    }
    updateCategories();
}

void BarChart::updateCategories() {
    if(dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->count() == dynamic_cast<BarChartModel*>(model)->getSeriesNumber()){
        for(int i = 0; i < dynamic_cast<BarChartModel*>(model)->getSeriesNumber(); ++i) {
            if(dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->at(i) != dynamic_cast<BarChartModel*>(model)->getSeriesAt(i)){
                dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->replace(dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->at(i), dynamic_cast<BarChartModel*>(model)->getSeriesAt(i));
            }
        }
    } else {
        dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->clear();
        dynamic_cast<QBarCategoryAxis*>(axes(Qt::Horizontal).at(0))->append(dynamic_cast<BarChartModel*>(model)->getAllSeries());

    }
}

void BarChart::updateColors() {
    for(int i = 0; i < bar_sets->length(); ++i) {
        bar_sets->at(i)->setColor(model->getColorAt(i));
    }
}

void BarChart::updateChart() {
    removeAxis(axes(Qt::Horizontal).at(0));
    addAxis(category_axis, Qt::AlignBottom);
    updateAxesYRange();
    loadFromModel();
}
