#include "../Header files/AbstractRowContainer.h"

AbstractRowContainer::AbstractRowContainer(QWidget *parent) {
    setParent(parent);
    setWidgetResizable(true);
    editing = false;

    rows = new QList<AbstractRow*>;

    container = new QWidget;
    layout = new QVBoxLayout(container);

    setWidget(container);
}

AbstractRowContainer::~AbstractRowContainer() {
    hide();
    delete layout;
    delete rows;
    delete container;
    takeWidget();
}

void AbstractRowContainer::appendRow(AbstractRow *row_to_append) {
    rows->append(row_to_append);
    layout->insertWidget(layout->count()-1,row_to_append);
    connectARow(*row_to_append);
    emit addedOneRow();
}

void AbstractRowContainer::connectARow(const AbstractRow &row) const {
    connect(row.delete_button, &QPushButton::clicked, this, &AbstractRowContainer::removeNodeFromList);
}

void AbstractRowContainer::removeNodeFromList() {
    for(int i = 0; i < rows->length(); ++i) {
        if(rows->at(i)->delete_button == sender()) {
            auto row_to_delete = rows->at(i);
            row_to_delete->hide();
            layout->removeWidget(row_to_delete);
            rows->removeAt(i);
            delete row_to_delete;
        }
    }
}

void AbstractRowContainer::resetRowButtons() const {
    if(rows->at(0)){
        if (editing) {
            showAllRowsButtons();
        }
    }
}

void AbstractRowContainer::editingMode(bool checked) {
    editing = checked;
    if(checked){
        showAllRowsButtons();
    } else{
        hideAllRowsButtons();
    }
}

void AbstractRowContainer::showAllRowsButtons() const {
    for(auto row : *rows) {
        row->showAllButtons();
    }
}

void AbstractRowContainer::hideAllRowsButtons() const {
    for(auto row : *rows) {
        row->hideAllButtons();
    }
}

void AbstractRowContainer::removeRowAt(int index) {
    auto row_to_delete = rows->at(index);
    row_to_delete->hide();
    layout->removeWidget(row_to_delete);
    rows->removeAt(index);
    delete row_to_delete;
    emit addedOneRow();
}