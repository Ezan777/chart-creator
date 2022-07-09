#ifndef PROGETTOPAO_SERIESCONTAINER_H
#define PROGETTOPAO_SERIESCONTAINER_H
#include "AbstractRowContainer.h"
#include "../../Rows/Header files/SeriesRow.h"


class SeriesContainer: public AbstractRowContainer {
    Q_OBJECT
private:
    AbstractModel *model;
public:
    explicit SeriesContainer(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~SeriesContainer() override;

    void loadContainerFromModel();
public slots:
    void createNewRow() override;
};


#endif //PROGETTOPAO_SERIESCONTAINER_H
