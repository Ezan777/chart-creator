#include "../Header files/SeriesContainer.h"

SeriesContainer::SeriesContainer(AbstractModel *chart_model, QWidget *parent) : AbstractRowContainer(parent) {
    this->model = chart_model;

    setMinimumWidth(180);

    layout->addStretch(100);
}

SeriesContainer::~SeriesContainer() {
    for(int i = rows->length(); i > 0; i -= 1) {
        layout->itemAt(i-1)->widget();
        delete layout->itemAt(i-1)->widget();
    }
}

void SeriesContainer::loadContainerFromModel() {
    for(int i = 0; i < dynamic_cast<BarChartModel*>(model)->getSeriesNumber(); ++i) {
        auto new_row = new SeriesRow(i,dynamic_cast<BarChartModel*>(model), this);
        appendRow(new_row);
        resetRowButtons();
        new_row->loadRowFromModel();
    }
}

void SeriesContainer::createNewRow() {
    auto *new_row = new SeriesRow(dynamic_cast<BarChartModel*>(model));
    appendRow(new_row);
    resetRowButtons();
}