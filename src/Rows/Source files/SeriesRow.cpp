#include "../Header files/SeriesRow.h"

SeriesRow::SeriesRow(BarChartModel *chart_model, QWidget *parent) : AbstractRow(parent) {
    setParent(parent);

    model = chart_model;

    series_name = new QLineEdit;
    series_name->setPlaceholderText(tr("Insert a name for the serie"));
    series_name->setFont(QFont("Arial",11));
    model_index = model->appendSeries(series_name->text());

    connect(series_name,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
    connect(series_name,&QLineEdit::textChanged,this,&SeriesRow::saveSerieToModel);
    connect(model, &BarChartModel::seriesRowDeleted, this, &SeriesRow::changeIndex);

    AbstractRow::layout->insertWidget(1,series_name);
}

SeriesRow::SeriesRow(int index, BarChartModel *chart_model, QWidget *parent) {
    setParent(parent);

    this->model = chart_model;
    model_index = index;

    series_name = new QLineEdit;
    series_name->setPlaceholderText(tr("Insert a name for the serie"));
    series_name->setFont(QFont("Arial",11));

    connect(series_name,&QLineEdit::textChanged,this,&AbstractRow::dataChanged);
    connect(series_name,&QLineEdit::textChanged,this,&SeriesRow::saveSerieToModel);
    connect(model, &BarChartModel::seriesRowDeleted, this, &SeriesRow::changeIndex);

    AbstractRow::layout->insertWidget(1,series_name);
}

SeriesRow::~SeriesRow() {
    model->removeSeriesAt(model_index);
    delete series_name;
}

void SeriesRow::setIndex(int i) {
    model_index = i;
}

void SeriesRow::saveSerieToModel(const QString &new_serie) const {
    model->editExistingSeries(model_index, new_serie);
}

void SeriesRow::loadRowFromModel() {
    series_name->setText(model->getSeriesAt(model_index));
}

void SeriesRow::saveRow(QXmlStreamWriter *stream) const {
    stream->writeTextElement("SerieName", series_name->text());
}

void SeriesRow::loadRow(QXmlStreamReader *stream) {
    QXmlStreamReader::TokenType token = stream->readNext();
    while(token != QXmlStreamReader::EndElement){
        if(token == QXmlStreamReader::StartElement){
            if(stream->name() == "SerieName"){
                series_name->setText(stream->readElementText());
                return;
            }
        }
        token = stream->readNext();
    }
}

void SeriesRow::changeIndex(int index) {
    if(index < model_index) {
        model_index -= 1;
    }
}