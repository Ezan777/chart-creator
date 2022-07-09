#ifndef PROGETTOPAO_PIEDATAVIEW_H
#define PROGETTOPAO_PIEDATAVIEW_H
#include "QWidget"
#include "../../Containers/Header files/RowContainer.h"
#include "QLabel"
#include "QPushButton"
#include "../../Models/Header files/PieChartModel.h"
#include "AbstractDataView.h"

class PieDataView: public AbstractDataView {
    Q_OBJECT
private:
    RowContainer *rows_container;
public:
    explicit PieDataView(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~PieDataView() override;

    void connectContainer() const;
public slots:
    void loadDataViewFromModel() override;
};


#endif //PROGETTOPAO_PIEDATAVIEW_H
