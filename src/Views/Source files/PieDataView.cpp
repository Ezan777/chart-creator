#include "../Header files/PieDataView.h"

PieDataView::PieDataView(AbstractModel *chart_model, QWidget *parent): AbstractDataView(chart_model, parent) {
    rows_container = new RowContainer(chart_model, parent);

    AbstractDataView::layout->insertWidget(1, rows_container);

    connectContainer();
}

PieDataView::~PieDataView() {
    delete rows_container;
}

void PieDataView::connectContainer() const {
    connect(new_line,&QPushButton::clicked,rows_container, &RowContainer::createNewRow);
    connect(edit_button, &QPushButton::toggled, rows_container, &RowContainer::editingMode);
    connect(rows_container, &RowContainer::editingChanged, edit_button, &QAbstractButton::setChecked);

}

void PieDataView::loadDataViewFromModel() {
    delete rows_container;
    rows_container = new RowContainer(true, 0, model);
    connectContainer();
    rows_container->loadContainerFromModel();
    layout->insertWidget(1, rows_container);
}