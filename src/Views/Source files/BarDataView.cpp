#include "../Header files/BarDataView.h"

BarDataView::BarDataView(AbstractModel *chart_model, QWidget *parent): AbstractDataView(chart_model, parent) {
    series_tab = new QTabWidget(this);
    containers_list = new QList<RowContainer*>;
    series_tab->clear();

    layout->insertWidget(1, series_tab);

    connect(dynamic_cast<BarChartModel*>(model), &BarChartModel::seriesChanged, this, &BarDataView::upgradeTabs);
}

BarDataView::~BarDataView() {
    for(int i = 0; i < series_tab->count(); ++i) {
        delete series_tab->widget(i);
        series_tab->removeTab(i);
    }
    delete series_tab;
    delete containers_list;
}

void BarDataView::loadDataViewFromModel() {
    auto bar_model = dynamic_cast<BarChartModel*>(model);
    series_tab->clear();
    containers_list->clear();
    int containers_number = bar_model->getSeriesNumber();
    for(int i = 0; i < containers_number; ++i) {
        auto new_container = new RowContainer(true,i, bar_model);
        containers_list->append(new_container);
        connectContainerAt(i);
        series_tab->addTab(new_container, bar_model->getSeriesAt(i));
        new_container->loadContainerFromModel();
    }
}

void BarDataView::removeRowFromAllContainersAt(int index) {
    for(auto i : *containers_list) {
        if(i != sender()) {
            i->removeRowAt(index);
        }
    }
}

void BarDataView::upgradeTabs() {
    auto bar_model = dynamic_cast<BarChartModel*>(model);
    if(bar_model->getSeriesNumber() > series_tab->count()) {
        // Added one serie
        if (containers_list->length() != 0) {
            //It's not the first container
            containers_list->append(new RowContainer(false, containers_list->length(), bar_model));
            connectContainerAt(containers_list->length() - 1);
            series_tab->addTab(containers_list->last(), bar_model->getSeriesAt(bar_model->getSeriesNumber() - 1));
        } else {
            //It's the first container
            containers_list->append(new RowContainer(bar_model));
            connectContainerAt(containers_list->length() - 1);
            series_tab->addTab(containers_list->last(), bar_model->getSeriesAt(bar_model->getSeriesNumber() - 1));
        }
    } else {
        if(bar_model->getSeriesNumber() < series_tab->count()) {
            // Deleted one serie
            for(int i = 0; i < series_tab->count(); ++i) {
                if(i != series_tab->count()-1) {
                    // Not on the last
                    if(series_tab->tabText(i) != bar_model->getSeriesAt(i)) {
                        delete series_tab->widget(i);
                        //series_tab->removeTab(i);
                    }
                } else {
                    // I am on the last tab, and I have got a series less, so I can't use getSeriesAt(i)
                    if (bar_model->getSeriesNumber() != 0) {
                        if (series_tab->tabText(i) != bar_model->getSeriesAt(bar_model->getSeriesNumber() - 1)) {
                            delete series_tab->widget(i);
                            //series_tab->removeTab(i);
                        }
                    } else {
                        delete series_tab->widget(i);
                    }
                }
            }
        } else {
            // Same number of series, it means I changed one serie's name
            for(int i = 0; i < series_tab->count(); ++i) {
                if(series_tab->tabText(i) != bar_model->getSeriesAt(i)) {
                    series_tab->setTabText(i, bar_model->getSeriesAt(i));
                }
            }
        }
    }
}

void BarDataView::connectContainerAt(int index) const {
    connect(new_line,&QPushButton::clicked,containers_list->at(index), &RowContainer::createNewRow);
    connect(edit_button, &QPushButton::toggled, containers_list->at(index), &RowContainer::editingMode);
    connect(containers_list->at(index), &RowContainer::editingChanged, edit_button, &QAbstractButton::setChecked);
    connect(containers_list->at(index),&AbstractRowContainer::rowDeletedAt,this,&BarDataView::removeRowFromAllContainersAt);
}