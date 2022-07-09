#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"

#include <QChart>
#include "../Header files/ChartView.h"
ChartView::ChartView(AbstractModel *chart_model, QWidget *parent) {
    setParent(parent);
    model = chart_model;

    layout = new QVBoxLayout;

    chart_view = new QtCharts::QChartView;

    if(dynamic_cast<BarChartModel*>(model)) {
        chart = new BarChart(dynamic_cast<BarChartModel*>(model), this);
    } else {
        if(dynamic_cast<PieChartModel*>(model)) {
            chart = new PieChart(dynamic_cast<PieChartModel*>(model), this);
        } else {
            if(dynamic_cast<SplineChartModel*>(model)) {
                chart = new SplineChart(dynamic_cast<SplineChartModel*>(model), this);
            }
        }
    }

    title_font = new QFont("Arial", 14, QFont::Bold);

    chart->setTitleFont(*title_font);
    chart_view->setChart(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);

    title_layout = new QHBoxLayout;

    title_head = new QLabel(tr("Chart Title:"),this);
    title_head->setFont(*title_font);

    title = new QLineEdit(this);
    title->setPlaceholderText(tr("Insert a title for the chart"));

    title_layout->addWidget(title_head);
    title_layout->addWidget(title);

    if (model) {
        connect(title, &QLineEdit::textChanged, model, &AbstractModel::setChartTitle);
        connect(model, &AbstractModel::titleChanged, this, &ChartView::changeChartTitle);
    }

    if (model) {
        if (model->getChartTitle() != "") {
            title->setText(model->getChartTitle());
        }
    }

    layout->addLayout(title_layout);
    layout->addWidget(chart_view);

    setLayout(layout);
    setMinimumWidth(500);
    setMinimumHeight(300);
}

ChartView::~ChartView() {
    delete layout;
    delete chart_view;
    delete title;
    delete title_head;
    delete title_font;
}

void ChartView::updateChartView() {
    chart->updateChart();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
void ChartView::saveChartAsImage() {
#ifdef __linux__
    QDir default_folder(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Chart Creator Projects");
    default_folder.mkdir("Images");
    QString file_path = QFileDialog::getSaveFileName(nullptr, tr("Save chart as"),
                                                     QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                                     +"/Chart Creator Projects"+"/Images",
                                                     tr("Image (*.png *.jpg)"), nullptr,
                                                     QFileDialog::DontUseNativeDialog);
#else
    QString file_path = QFileDialog::getSaveFileName(nullptr, tr("Save chart as"),
                                                     QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                                     +"/Chart Creator Projects"+"/Images",
                                                     tr("Image (*.png *.jpg)"), nullptr);
#endif

    if(file_path == ""){
        return;
    }
    QString file_suffix = QFileInfo(file_path).completeSuffix();
    if(!file_suffix.contains("png")) {
        if(!file_suffix.contains("jpg")) {
            file_path.append(".png");
        }
    }

    const qreal dpr = chart_view->devicePixelRatioF();

    QPixmap image(chart_view->width() * dpr, chart_view->height() * dpr);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);
    QPainter paint(&image);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setPen(QColor(255,34,255,255));
    chart_view->render(&paint);
    image.toImage();
    if(!image.save(file_path, nullptr, 100)) {
        throw std::exception();
    }
}
#pragma clang diagnostic pop

void ChartView::changeChartTitle(const QString &new_title) {
    chart->setTitle(new_title);
}

void ChartView::darkMode() {
    chart->darkMode();
    chart->setTitleFont(*title_font);
}

void ChartView::lightMode() {
    chart->lightMode();
    chart->setTitleFont(*title_font);
}

#pragma clang diagnostic pop