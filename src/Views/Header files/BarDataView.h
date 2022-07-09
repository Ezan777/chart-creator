#ifndef PROGETTOPAO_BARDATAVIEW_H
#define PROGETTOPAO_BARDATAVIEW_H
#include "QWidget"
#include "../../Containers/Header files/RowContainer.h"
#include "QLabel"
#include "QPushButton"
#include "QTabWidget"
#include "../../Models/Header files/BarChartModel.h"
#include "AbstractDataView.h"

class BarDataView: public AbstractDataView {
    Q_OBJECT
public:
    explicit BarDataView(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~BarDataView() override;
    void connectContainerAt(int index) const;
private:
    QTabWidget *series_tab;
    QList<RowContainer*> *containers_list;
public slots:
    void loadDataViewFromModel();
    void removeRowFromAllContainersAt(int index);
    void upgradeTabs();
};


#endif //PROGETTOPAO_BARDATAVIEW_H
