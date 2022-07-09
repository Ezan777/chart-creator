#include "../Header files/SplineRowContainer.h"

SplineRowContainer::SplineRowContainer(AbstractModel *chart_model, QWidget *parent): AbstractRowContainer(parent) {
    model = model = chart_model;
    AbstractRowContainer::editing = false;

    AbstractRowContainer::layout->addStretch(100);
}

SplineRowContainer::~SplineRowContainer() = default;

void SplineRowContainer::loadContainerFromModel() {
    int c = model->getNumberOfContainersRows();
    for(int i = 0; i < model->getNumberOfContainersRows(); ++i) {
        AbstractRow *new_row = new SplineDataRow(true, model, i);
        appendRow(new_row);
        resetRowButtons();
        new_row->loadRowFromModel();
    }
}

void SplineRowContainer::createNewRow() {
    AbstractRow *new_row = new SplineDataRow(false, model, 0);
    appendRow(new_row);
    resetRowButtons();
}