#ifndef PROGETTOPAO_SPLINEDATAVIEW_H
#define PROGETTOPAO_SPLINEDATAVIEW_H


#include "AbstractDataView.h"
#include "../../Containers/Header files/SplineRowContainer.h"

class SplineDataView: public AbstractDataView {
    Q_OBJECT
private:
    SplineRowContainer *rows_container;
public:
    explicit SplineDataView(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~SplineDataView() override;

    void connectContainer() const;
public slots:
    void loadDataViewFromModel() override;
};


#endif //PROGETTOPAO_SPLINEDATAVIEW_H
