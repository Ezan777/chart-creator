#ifndef PROGETTOPAO_ROWCONTAINER_H
#define PROGETTOPAO_ROWCONTAINER_H
#include "QWidget"
#include "../../Rows/Header files/DataRow.h"
#include "QScrollArea"
#include "QDebug"
#include "AbstractRowContainer.h"
#include "../../Models/Header files/BarChartModel.h"


class RowContainer: public AbstractRowContainer {
    Q_OBJECT
private:
    AbstractModel *model;
    int container_index;
public:
    explicit RowContainer(AbstractModel *chart_model, QWidget *parent = nullptr);
    explicit RowContainer(bool on_load, int index, AbstractModel *chart_model, QWidget *parent = nullptr);
    ~RowContainer() override;
    void loadContainerFromModel();
    void removeFromLayoutAt(int index);
public slots:
    void createNewRow() override;
    void removeNodeFromList() override;
    void removeRowAt(int index) override;
    void changeColorsButtonAt(int index);
    signals:
    void creatingARow();
    void removingARowAt();
};


#endif //PROGETTOPAO_ROWCONTAINER_H
