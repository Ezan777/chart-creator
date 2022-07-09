#include "../Header files/SplineDataRow.h"

SplineDataRow::SplineDataRow(bool on_load, AbstractModel *chart_model, int index, QWidget *parent)
        : AbstractRow(parent) {
    if (!on_load) {
        model = chart_model;
        abscissa = new QLineEdit(this);
        ordinate = new QLineEdit(this);
        data_validator = new QDoubleValidator;

        abscissa->setValidator(data_validator);
        ordinate->setValidator(data_validator);

        abscissa->setPlaceholderText(tr("Insert the abscissa (numbers only)"));
        ordinate->setPlaceholderText(tr("Insert the ordinate (numbers only)"));

        model_index = model->appendLabel(abscissa->text());
        model->appendData(ordinate->text());

        connect(abscissa, &QLineEdit::textChanged, this, &SplineDataRow::saveAbscissaToModel);
        connect(ordinate, &QLineEdit::textChanged, this, &SplineDataRow::saveOrdinateToModel);
        connect(model, &AbstractModel::dataRowDeleted, this, &SplineDataRow::changeIndex);

        AbstractRow::layout->addWidget(abscissa);
        AbstractRow::layout->addWidget(ordinate);
    } else {
        model = chart_model;
        abscissa = new QLineEdit(this);
        ordinate = new QLineEdit(this);
        data_validator = new QDoubleValidator;

        abscissa->setValidator(data_validator);
        ordinate->setValidator(data_validator);

        abscissa->setPlaceholderText(tr("Insert the abscissa (numbers only)"));
        ordinate->setPlaceholderText(tr("Insert the ordinate (numbers only)"));

        model_index = index;

        connect(abscissa, &QLineEdit::textChanged, this, &SplineDataRow::saveAbscissaToModel);
        connect(ordinate, &QLineEdit::textChanged, this, &SplineDataRow::saveOrdinateToModel);
        connect(model, &AbstractModel::dataRowDeleted, this, &SplineDataRow::changeIndex);

        AbstractRow::layout->addWidget(abscissa);
        AbstractRow::layout->addWidget(ordinate);
    }
}

SplineDataRow::~SplineDataRow() {
    model->removeLabelAt(model_index);
    model->removeDataAt(model_index);
    delete abscissa;
    delete ordinate;
}

void SplineDataRow::loadRowFromModel() {
    abscissa->setText(model->getLabelAt(model_index));
    ordinate->setText(model->getDataAt(model_index));
}

void SplineDataRow::saveAbscissaToModel() const {
    model->editExistingLabel(model_index, abscissa->text());
}

void SplineDataRow::saveOrdinateToModel() const {
    model->editExistingData(model_index, ordinate->text());
}

void SplineDataRow::changeIndex(int index) {
    if(index < model_index) {
        model_index -= 1;
    }
}

void SplineDataRow::saveRow(QXmlStreamWriter *stream) const {}

void SplineDataRow::loadRow(QXmlStreamReader *stream) {}