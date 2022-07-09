#ifndef PROGETTOPAO_ABSTRACTDATAVIEW_H
#define PROGETTOPAO_ABSTRACTDATAVIEW_H

#include "QWidget"
#include "../../Containers/Header files/RowContainer.h"
#include "QLabel"
#include "QPushButton"
#include "QTabWidget"
#include "../../Models/Header files/BarChartModel.h"


class AbstractDataView: public QWidget {
    Q_OBJECT
protected:
    QLabel *label_head;
    QLabel *data_label_head;
    AbstractModel *model;
    QPushButton *new_line;
    QHBoxLayout *heads_layout;
    QVBoxLayout *layout;
    QPushButton *edit_button;
    QHBoxLayout *buttons_layout;
public:
    explicit AbstractDataView(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~AbstractDataView() override;
public slots:
    virtual void loadDataViewFromModel() = 0;
};


#endif //PROGETTOPAO_ABSTRACTDATAVIEW_H
