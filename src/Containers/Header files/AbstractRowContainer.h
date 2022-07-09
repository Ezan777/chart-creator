#ifndef PROGETTOPAO_ABSTRACTROWCONTAINER_H
#define PROGETTOPAO_ABSTRACTROWCONTAINER_H
#include "QScrollArea"
#include "../../Rows/Header files/AbstractRow.h"
#include "../../Rows/Header files/DataRow.h"

class AbstractRowContainer: public QScrollArea {
    Q_OBJECT
protected:
    bool editing;
    QList<AbstractRow*> *rows;
    QVBoxLayout *layout;
    QWidget *container;
    void appendRow(AbstractRow *row_to_append);
public:
    explicit AbstractRowContainer(QWidget *parent = nullptr);
    ~AbstractRowContainer() override;
    void connectARow(const AbstractRow &row) const;
public slots:
    virtual void createNewRow() = 0;
    virtual void removeNodeFromList();
    void resetRowButtons() const;
    void editingMode(bool checked);
    void showAllRowsButtons() const;
    void hideAllRowsButtons() const;
    virtual void removeRowAt(int index);
signals:
    void editingChanged(bool editing);
    void rowDeletedAt(int index);
    void addedOneRow();
};


#endif //PROGETTOPAO_ABSTRACTROWCONTAINER_H
