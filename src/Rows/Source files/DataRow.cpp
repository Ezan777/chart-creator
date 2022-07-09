#include <QDoubleValidator>
#include "../Header files/DataRow.h"

QValidator* DataRow::data_validator = new QDoubleValidator;

DataRow::DataRow(int container_index, AbstractModel *chart_model, QWidget *parent)
        : AbstractRow(parent) {
    setParent(parent);

    model = chart_model;
    this->container_index = container_index;

    label = new QLineEdit(this);
    label->setPlaceholderText(tr("Insert a label"));
    label_index = model->appendLabel(label->text());
    data = new QLineEdit(this);
    data->setValidator(data_validator);
    data->setPlaceholderText(tr("Insert data (numbers only)"));
    data_index = label_index;
    model->insertData(label_index, data->text());

    color_picker = new SelectColorButton(this);
    //Generate a random color
    std::uniform_int_distribution<int> color_distribution(100000,999999);
    std::random_device color_device;
    color_picker->setColor("#" + QString::number(color_distribution(color_device)));
    model->appendColor(color_picker->getColor());
    connect(color_picker, &SelectColorButton::colorChanged, this, &DataRow::saveColorToModel);

    connect(label,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
    connect(label,&QLineEdit::textChanged,this,&DataRow::saveLabelToModel);
    connect(model, &BarChartModel::labelsChanged, this, &DataRow::upgradeLabel);
    connect(data,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
    connect(data,&QLineEdit::textChanged,this,&DataRow::saveDataToModel);
    connect(model, &BarChartModel::dataRowDeleted, this, &DataRow::changeIndex);

    AbstractRow::layout->insertWidget(1, color_picker);
    color_picker->hide();
    AbstractRow::layout->insertWidget(2,label);
    AbstractRow::layout->insertWidget(3,data);
}

DataRow::DataRow(bool on_load, int index, int container_index, AbstractModel *chart_model, QWidget *parent) {
    if (on_load) {
        setParent(parent);
        model = chart_model;
        this->container_index = container_index;

        label = new QLineEdit(this);
        label->setPlaceholderText(tr("Insert a label"));
        label_index = index;
        connect(label, &QLineEdit::textChanged, this, &AbstractRow::dataChanged);
        connect(label, &QLineEdit::textChanged, this, &DataRow::saveLabelToModel);
        connect(model, &BarChartModel::labelsChanged, this, &DataRow::upgradeLabel);
        connect(model, &BarChartModel::dataRowDeleted, this, &DataRow::changeIndex);

        data = new QLineEdit(this);
        data->setValidator(data_validator);
        data->setPlaceholderText(tr("Insert data (numbers only)"));
        data_index = model->getNumberOfContainersRows() * container_index + label_index;
        connect(data, &QLineEdit::textChanged, this, &AbstractRow::dataChanged);
        connect(data, &QLineEdit::textChanged, this, &DataRow::saveDataToModel);

        color_picker = new SelectColorButton(this);
        //Generate a random color
        std::uniform_int_distribution<int> color_distribution(100000,999999);
        std::random_device color_device;
        color_picker->setColor(model->getColorAt(label_index));
        connect(color_picker, &SelectColorButton::colorChanged, this, &DataRow::saveColorToModel);

        AbstractRow::layout->insertWidget(1, color_picker);
        color_picker->hide();
        AbstractRow::layout->insertWidget(2, label);
        AbstractRow::layout->insertWidget(3, data);
    } else {
        setParent(parent);

        model = chart_model;
        this->container_index = container_index;

        label = new QLineEdit(this);
        label->setPlaceholderText(tr("Insert a label"));
        label_index = index;
        connect(label,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
        connect(label,&QLineEdit::textChanged,this,&DataRow::saveLabelToModel);
        connect(model, &BarChartModel::labelsChanged, this, &DataRow::upgradeLabel);
        connect(model, &BarChartModel::dataRowDeleted, this, &DataRow::changeIndex);

        data = new QLineEdit(this);
        data->setValidator(data_validator);
        data->setPlaceholderText(tr("Insert data (numbers only)"));
        data_index = index + container_index * model->getNumberOfContainersRows();
        model->insertData(data_index, data->text());
        connect(data,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
        connect(data,&QLineEdit::textChanged,this,&DataRow::saveDataToModel);

        color_picker = new SelectColorButton(this);
        //Generate a random color
        std::uniform_int_distribution<int> color_distribution(100000,999999);
        std::random_device color_device;
        color_picker->setColor(model->getColorAt(label_index));
        connect(color_picker, &SelectColorButton::colorChanged, this, &DataRow::saveColorToModel);

        AbstractRow::layout->insertWidget(1, color_picker);
        color_picker->hide();
        AbstractRow::layout->insertWidget(2,label);
        AbstractRow::layout->insertWidget(3,data);
    }
}

DataRow::~DataRow() {
    label->hide();
    data->hide();
    color_picker->hide();
    layout->removeWidget(label);
    layout->removeWidget(data);
    layout->removeWidget(color_picker);
    delete label;
    delete data;
    delete color_picker;
}

void DataRow::changeIndex(int index) {
    if(index < label_index){
        label_index -= 1;
        data_index -= 1;
    }
}

QString DataRow::getLabel() const {
    return label->text();
}

double DataRow::getData() const {
    return data->text().toDouble();
}

void DataRow::saveRow(QXmlStreamWriter *stream) const {
    stream->writeTextElement("Label", label->text());
    stream->writeTextElement("Data",data->text());
}

void DataRow::loadRow(QXmlStreamReader *stream) {
    QXmlStreamReader::TokenType token = stream->readNext();
    while (token != QXmlStreamReader::EndDocument){
        if(stream->name() == "Label"){
            label->setText(stream->readElementText());
        } else{
            if(stream->name() == "Data"){
                data->setText(QString::number((stream->readElementText()).toDouble()));
                return;
            }
        }
        token = stream->readNext();
    }
}

void DataRow::removeRowLabelFromModel() const {
    model->removeLabelAt(label_index);
}

void DataRow::removeRowDataFromModel() const {
    model->removeDataAt(data_index - container_index);
}

void DataRow::showAllButtons() const {
    AbstractRow::showAllButtons();
    color_picker->show();
}

void DataRow::hideAllButtons() const {
    AbstractRow::hideAllButtons();
    color_picker->hide();
}

void DataRow::saveLabelToModel(const QString& new_label) const {
    model->editExistingLabel(label_index, new_label);
}

void DataRow::saveColorToModel(const QString &new_color) const {
    model->editExistingColor(label_index, new_color);
}

void DataRow::saveDataToModel(const QString& new_data) const {
    model->editExistingData(data_index, new_data);
}

void DataRow::loadRowFromModel() {
    label->setText(model->getLabelAt(label_index));
    data->setText(model->getDataAt(data_index));
}

void DataRow::upgradeLabel(int index) {
    if(label_index == index) {
        label->setText(model->getLabelAt(index));
    }
}

void DataRow::increaseDataIndex() {
    data_index += container_index;
}

void DataRow::decreaseDataIndex() {
    data_index -= container_index;
}