#ifndef PROGETTOPAO_SPLINEROWCONTAINER_H
#define PROGETTOPAO_SPLINEROWCONTAINER_H


#include "AbstractRowContainer.h"
#include "../../Rows/Header files/SplineDataRow.h"

class SplineRowContainer: public AbstractRowContainer {
    Q_OBJECT
private:
    AbstractModel *model;
public:
    explicit SplineRowContainer(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~SplineRowContainer() override;

    void loadContainerFromModel();
public slots:
    void createNewRow() override;
};


#endif //PROGETTOPAO_SPLINEROWCONTAINER_H
