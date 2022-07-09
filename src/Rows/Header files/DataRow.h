#ifndef PROGETTOPAO_DATAROW_H
#define PROGETTOPAO_DATAROW_H
#include "QWidget"
#include "QLineEdit"
#include "QPushButton"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QXmlStreamReader"
#include "QXmlStreamWriter"
#include "QDebug"
#include "AbstractRow.h"
#include "../../Models/Header files/BarChartModel.h"
#include "../../Buttons/Header files/SelectColorButton.h"
#include <random>


class DataRow: public AbstractRow {
    Q_OBJECT
    friend class RowContainer;
private:
    QLineEdit *data;
    AbstractModel *model;
    int label_index;
    int data_index;
    int container_index;
protected:
    SelectColorButton *color_picker;
    QLineEdit *label;
    static QValidator *data_validator;
public:
    explicit DataRow(int container_index, AbstractModel *chart_model, QWidget *parent = nullptr);
    explicit DataRow(bool on_load, int index, int container_index, AbstractModel *chart_model, QWidget *parent = nullptr);
    ~DataRow() override;
    QString getLabel() const;
    double getData() const;
    void removeRowLabelFromModel() const;
    void removeRowDataFromModel() const;
    void showAllButtons() const override;
    void hideAllButtons() const override;
public slots:
    void changeIndex(int index);
    void upgradeLabel(int index);
    void saveRow(QXmlStreamWriter *stream) const override;
    void loadRow(QXmlStreamReader *stream) override;
    void saveLabelToModel(const QString& new_label) const;
    void saveColorToModel(const QString& new_color) const;
    void saveDataToModel(const QString& new_data) const;
    void loadRowFromModel() override;
    void increaseDataIndex();
    void decreaseDataIndex();
    signals:
    void deleteCopies();
    void moveDownCopies();
    void moveUpCopies();
};


#endif //PROGETTOPAO_DATAROW_H
