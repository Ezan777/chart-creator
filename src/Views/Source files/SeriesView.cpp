#include "../Header files/SeriesView.h"

SeriesView::SeriesView(AbstractModel *chart_model, QWidget *parent) {
    setParent(parent);

    model = chart_model;
    series_container = new SeriesContainer(model, this);

    add_row = new QPushButton(QIcon(":/icons/new_circle.png"),tr("New line"),this);
    edit_button = new QPushButton(QIcon(":/icons/edit_pencil_black.png"),tr("Edit"),this);
    edit_button->setCheckable(true);
    edit_button->setChecked(false);

    connectContainer();

    buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(edit_button);
    buttons_layout->addWidget(add_row);

    series_label = new QLabel(tr("Categories"),this);
    series_label->setFont(QFont("Arial",14,QFont::Bold));

    layout = new QVBoxLayout(this);
    layout->addWidget(series_label);
    layout->addWidget(series_container);
    layout->addLayout(buttons_layout);

    setMinimumWidth(200);
    setMaximumWidth(270);
    setMinimumHeight(300);
}

SeriesView::~SeriesView() {
    delete series_container;
    delete add_row;
    delete edit_button;
    delete series_label;
    delete layout;
}

void SeriesView::loadSeriesViewFromModel() {
    delete series_container;
    series_container = new SeriesContainer(model, this);
    layout->insertWidget(layout->count()-1,series_container);
    series_container->loadContainerFromModel();
    connectContainer();
}

void SeriesView::connectContainer() const {
    connect(add_row,&QPushButton::clicked,series_container,&SeriesContainer::createNewRow);
    connect(edit_button,&QPushButton::toggled,series_container,&SeriesContainer::editingMode);
    connect(series_container,&SeriesContainer::editingChanged,edit_button,&QPushButton::setChecked);

}