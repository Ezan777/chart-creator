#include "../Header files/RowContainer.h"
RowContainer::RowContainer(AbstractModel *chart_model, QWidget *parent) {
    setParent(parent);
    setWidgetResizable(true);
    editing = false;

    model = chart_model;

    container = new QWidget;
    container_index = 0;

    layout = new QVBoxLayout(container);
    layout->addStretch(100);
    setMinimumWidth(180);

    setWidget(container);

    connect(model, &AbstractModel::updateColorsButtonsAt, this, &RowContainer::changeColorsButtonAt);
}

RowContainer::RowContainer(bool on_load, int index, AbstractModel *chart_model, QWidget *parent) {
    if (on_load) {
        setParent(parent);
        setWidgetResizable(true);
        editing = false;

        model = chart_model;

        container = new QWidget;

        container_index = index;

        layout = new QVBoxLayout(container);
        layout->addStretch(100);
        setMinimumWidth(180);

        for (int i = 0; i < model->getNumberOfContainersRows(); ++i) {
            AbstractRow *new_row = new DataRow(on_load, i, container_index, model);
            appendRow(new_row);
            connect(this, &RowContainer::creatingARow, dynamic_cast<DataRow *>(new_row), &DataRow::increaseDataIndex);
            connect(this, &RowContainer::removingARowAt, dynamic_cast<DataRow *>(new_row), &DataRow::decreaseDataIndex);
            layout->insertWidget(i, rows->at(i));
        }

        connect(model, &AbstractModel::updateColorsButtonsAt, this, &RowContainer::changeColorsButtonAt);
        setWidget(container);
    } else {
        setParent(parent);
        setWidgetResizable(true);
        editing = false;

        model = chart_model;

        container = new QWidget;

        container_index = index;

        layout = new QVBoxLayout(container);
        layout->addStretch(100);
        setMinimumWidth(180);

        for(int i = 0; i < model->getNumberOfContainersRows(); ++i) {
            AbstractRow *new_row = new DataRow(on_load, i, container_index, model);
            appendRow(new_row);
            connect(this, &RowContainer::creatingARow, dynamic_cast<DataRow*>(new_row), &DataRow::increaseDataIndex);
            connect(this, &RowContainer::removingARowAt, dynamic_cast<DataRow*>(new_row), &DataRow::decreaseDataIndex);
            layout->insertWidget(i,rows->at(i));
        }

        connect(model, &AbstractModel::updateColorsButtonsAt, this, &RowContainer::changeColorsButtonAt);
        setWidget(container);
    }
}

RowContainer::~RowContainer() {
    for(int i = rows->length(); i > 0; i -= 1) {
        delete layout->itemAt(i-1)->widget();
    }
}

void RowContainer::loadContainerFromModel() {
    int number_of_rows = model->getNumberOfContainersRows();
    for(int i = 0; i < number_of_rows; ++i) {
        dynamic_cast<DataRow*>(rows->at(i))->loadRowFromModel();
    }
}

void RowContainer::createNewRow() {
    if (container_index == 0) {
        emit creatingARow();
        auto *new_row = new DataRow(container_index, model, container);
        appendRow(new_row);
        connect(this, &RowContainer::creatingARow, new_row, &DataRow::increaseDataIndex);
        connect(this, &RowContainer::removingARowAt, new_row, &DataRow::decreaseDataIndex);
        resetRowButtons();
    } else {
        emit creatingARow();
        auto *new_row = new DataRow(false,rows->length(),container_index, model, container);
        appendRow(new_row);
        connect(this, &RowContainer::creatingARow, new_row, &DataRow::increaseDataIndex);
        connect(this, &RowContainer::removingARowAt, new_row, &DataRow::decreaseDataIndex);
        resetRowButtons();
    }
}

void RowContainer::removeNodeFromList() {
    bool deleted = false;
    for(int i = 0; i < rows->length() && !deleted; ++i) {
        if(rows->at(i)->delete_button == sender()) {
            auto row_to_delete = rows->at(i);
            row_to_delete->hide();
            layout->removeWidget(row_to_delete);
            dynamic_cast<DataRow *>(row_to_delete)->removeRowLabelFromModel();
            model->removeDataAt(rows->length()*container_index +1);
            if(rows->length() - 1 == i && rows->length() != 1) {
                //This is the last row
                for (int j = 1; j <= i; ++j) {
                    dynamic_cast<DataRow *>(rows->at(i - j))->decreaseDataIndex();
                }
            } else {
                if(i == 0 && rows->length() != 1) {
                    //This is the first row and there are some other rows
                    for (int j = 1; j < rows->length()-1; ++j) {
                        dynamic_cast<DataRow *>(rows->at(i + j))->decreaseDataIndex();
                    }
                } else {
                    if(rows->length() != 1) {
                        //The row is somewhere in the middle
                        for (int j = 1; j <= i; ++j) {
                            dynamic_cast<DataRow *>(rows->at(i - j))->decreaseDataIndex();
                        }
                        for (int j = 1; j < rows->length()-i-1; ++j) {
                            dynamic_cast<DataRow *>(rows->at(i + j))->decreaseDataIndex();
                        }
                    }
                }
            }
            rows->removeAt(i);
            delete row_to_delete;
            emit rowDeletedAt(i);
            deleted = true;
        }
    }
}

void RowContainer::removeRowAt(int index) {
    rows->at(index)->hide();
    layout->removeWidget(rows->at(index));
    dynamic_cast<DataRow *>(rows->at(index))->removeRowDataFromModel();
    emit removingARowAt();
    delete rows->at(index);
    rows->removeAt(index);
}

void RowContainer::changeColorsButtonAt(int index) {
    dynamic_cast<DataRow*>(rows->at(index))->color_picker->updateColorFromModel(model->getColorAt(index));
}

void RowContainer::removeFromLayoutAt(int index) {
    auto layout_item = layout->itemAt(index);
    layout->removeItem(layout_item);
}