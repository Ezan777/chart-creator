#ifndef PROGETTOPAO_SPLINEDATAROW_H
#define PROGETTOPAO_SPLINEDATAROW_H


#include "DataRow.h"
#include "QDoubleValidator"

class SplineDataRow: public AbstractRow {
    Q_OBJECT
private:
    AbstractModel *model;
    QLineEdit *abscissa, *ordinate;
    QValidator *data_validator;
    int model_index;
public:
    explicit SplineDataRow(bool on_load, AbstractModel *chart_model, int index, QWidget *parent = nullptr);
    ~SplineDataRow() override;

    void loadRowFromModel() override;
public slots:
    void saveAbscissaToModel() const;
    void saveOrdinateToModel() const;
    void changeIndex(int index);

    virtual void saveRow(QXmlStreamWriter *stream) const override;
    virtual void loadRow(QXmlStreamReader *stream) override;
};


#endif //PROGETTOPAO_SPLINEDATAROW_H
