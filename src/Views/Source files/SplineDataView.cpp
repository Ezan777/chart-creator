#include "../Header files/SplineDataView.h"

SplineDataView::SplineDataView(AbstractModel *chart_model, QWidget *parent): AbstractDataView(chart_model, parent) {
    rows_container = new SplineRowContainer(chart_model, parent);

    AbstractDataView::label_head->setText(tr("Abscissa"));
    AbstractDataView::data_label_head->setText(tr("Ordinate"));

    AbstractDataView::layout->insertWidget(1, rows_container);

    connectContainer();
}

SplineDataView::~SplineDataView() {
    delete rows_container;
}

void SplineDataView::connectContainer() const {
    connect(new_line, &QPushButton::clicked, rows_container, &SplineRowContainer::createNewRow);
    connect(edit_button, &QPushButton::toggled, rows_container, &SplineRowContainer::editingMode);
}

void SplineDataView::loadDataViewFromModel() {
    delete rows_container;
    rows_container = new SplineRowContainer(model);
    connectContainer();
    rows_container->loadContainerFromModel();
    AbstractDataView::layout->insertWidget(1, rows_container);
}