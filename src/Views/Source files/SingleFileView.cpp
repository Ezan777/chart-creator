#include "../Header files/SingleFileView.h"

SingleFileView::SingleFileView(const QString &file_path, ChartType type, QWidget *parent) {
  setParent(parent);
  this->file_path = new QString(file_path.data());
  model = nullptr;

  tool_bar = new QToolBar(this);

  views_layout = new QHBoxLayout;

  chart_type = type;

  chart_view = nullptr;

  QIcon save_as_image_icon(":/icons/save_as_image.png");
  save_chart_as_image = new QAction(save_as_image_icon, tr("Save chart as image"));

  buildView();

  QIcon save_icon(":/icons/save.png");
  QIcon save_as_icon(":/icons/save_as.png");

  save_action = new QAction(save_icon, tr("Save"));
  connect(save_action, &QAction::triggered, this, &SingleFileView::save);
  tool_bar->insertAction(save_as_action, save_action);

  save_as_action = new QAction(save_as_icon, tr("Save as"));
  connect(save_as_action, &QAction::triggered, this, &SingleFileView::save);
  tool_bar->insertAction(save_chart_as_image, save_as_action);

  QIcon light_mode_icon(":/icons/light_mode.png");
  light_mode_action = new QAction(light_mode_icon, tr("Switch chart to light theme"));
  connect(light_mode_action, &QAction::triggered, this, &SingleFileView::enableLightMode);

  QIcon dark_mode_icon(":/icons/dark_mode.png");
  dark_mode_action = new QAction(dark_mode_icon, tr("Switch chart to dark theme"));
  connect(dark_mode_action, &QAction::triggered, this, &SingleFileView::enableDarkMode);
  light_mode = true;

  tool_bar->addAction(save_action);
  tool_bar->addAction(save_as_action);
  tool_bar->addAction(save_chart_as_image);
  tool_bar->addAction(dark_mode_action);

  tool_bar->insertSeparator(save_as_action);
  tool_bar->insertSeparator(save_chart_as_image);

  file_layout = new QVBoxLayout(this);
  file_layout->addWidget(tool_bar);
  file_layout->addLayout(views_layout);

  readSettings();

  if (!light_mode) {
    enableDarkMode();
  }
}

SingleFileView::~SingleFileView() {
  saveSettings();
  delete chart_view;
  delete data_view;
  delete series_view;
  delete model;
  delete views_layout;
  delete file_layout;
  delete tool_bar;
  delete save_action;
  delete save_as_action;
  delete save_chart_as_image;
  delete dark_mode_action;
  delete light_mode_action;
  delete file_path;
}

void SingleFileView::buildBarView() {
  //delete data_view;
  //delete series_view;

  data_view = new BarDataView(model, this);
  series_view = new SeriesView(dynamic_cast<BarChartModel *>(model), this);

  views_layout->insertWidget(0, data_view);
  views_layout->insertWidget(1, series_view);

  connect(dynamic_cast<BarChartModel *>(model),
          &BarChartModel::seriesChanged,
          dynamic_cast<BarDataView *>(data_view),
          &BarDataView::upgradeTabs);
}

void SingleFileView::buildPieView() {
  data_view = new PieDataView(model, this);
  series_view = nullptr;

  views_layout->insertWidget(0, data_view);
}

void SingleFileView::buildSplineView() {
  data_view = new SplineDataView(model, this);
  series_view = nullptr;

  views_layout->insertWidget(0, data_view);
}

void SingleFileView::buildView() {
  delete chart_view;
  switch (chart_type) {
    case ChartType::BarChart:
      if (!model) {
        model = new BarChartModel;
      }
      buildBarView();
      break;
    case ChartType::PieChart:
      if (!model) {
        model = new PieChartModel;
      }
      buildPieView();
      break;
    case ChartType::SplineChart:
      if (!model) {
        model = new SplineChartModel;
      }
      buildSplineView();
      break;
  }
  chart_view = new ChartView(model, this);
  connect(save_chart_as_image, &QAction::triggered, chart_view, &ChartView::saveChartAsImage);
  views_layout->insertWidget(2, chart_view);
}

void SingleFileView::readSettings() {
  QSettings settings;

  settings.beginGroup(*file_path);
  light_mode = settings.value("light-mode").toBool();
  settings.endGroup();
}

void SingleFileView::saveSettings() const {
  QSettings settings;

  settings.beginGroup(*file_path);
  if (light_mode) {
    settings.setValue("light-mode", true);
  } else {
    settings.setValue("light-mode", false);
  }
  settings.endGroup();
}

QString SingleFileView::getFilePath() const {
  return *file_path;
}

void SingleFileView::saveSingleFileToXML(QXmlStreamWriter *stream) const {
  model->saveModelToXML(stream);
}

void SingleFileView::saveSingleFileToJson(QJsonObject *json) const {
  model->saveModelToJson(json);
}

void SingleFileView::loadSingleFile(QString *file_path) {
  QFile file(*file_path);
  file.open(QIODevice::ReadOnly);
  if (QFileInfo(file).suffix() == "xml") {
    QXmlStreamReader stream(&file);
    delete data_view;
    delete series_view;
    ChartType file_type = AbstractModel::getChartTypeFromXML(&file);
    stream.clear();
    file.close();
    file.open(QIODevice::ReadOnly);
    stream.setDevice(&file);
    if (chart_type != file_type) {
      chart_type = file_type;
      delete model;
      model = nullptr;
      buildView();
      model->loadModelFromXML(&stream);
      delete chart_view;
      chart_view = new ChartView(model, this);
      views_layout->insertWidget(2, chart_view);
      connect(save_chart_as_image, &QAction::triggered, chart_view, &ChartView::saveChartAsImage);
    } else {
      model->loadModelFromXML(&stream);
      buildView();
    }

    data_view->loadDataViewFromModel();
    if (series_view) {
      series_view->loadSeriesViewFromModel();
    }
    chart_view->updateChartView();
  } else {
    if(QFileInfo(file).suffix() == "json") {
      delete data_view;
      delete series_view;
      QByteArray saved_data = file.readAll();
      QJsonDocument json_document = QJsonDocument::fromJson(saved_data);
      QJsonObject json_object = json_document.object();
      QString type_from_json = json_object["chartType"].toString();
      ChartType file_type;
      if(type_from_json == "BarChart") {
        file_type = ChartType::BarChart;
      } else {
        if(type_from_json == "PieChart") {
          file_type = ChartType::PieChart;
        } else {
          if(type_from_json == "SplineChart") {
            file_type = ChartType::SplineChart;
          }
        }
      }
      if(chart_type != file_type) {
        chart_type = file_type;
        delete model;
        model = nullptr;
        buildView();
        model->loadModelFromJson(json_object);
        delete chart_view;
        chart_view = new ChartView(model, this);
        views_layout->insertWidget(2, chart_view);
        connect(save_chart_as_image, &QAction::triggered, chart_view, &ChartView::saveChartAsImage);
      } else {
        model->loadModelFromJson(json_object);
        buildView();
      }
      data_view->loadDataViewFromModel();
      if (series_view) {
        series_view->loadSeriesViewFromModel();
      }
      chart_view->updateChartView();
    }
  }
  file.close();

  if (!light_mode) {
    enableDarkMode();
  }
}

void SingleFileView::save() {
  if (save_action == sender()) {
    emit saveSingleFileAs(*file_path);
  } else {
#ifdef __linux__
    QString path = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                QStandardPaths::writableLocation(
                                                    QStandardPaths::DocumentsLocation)
                                                    + "/Chart Creator Projects",
                                                tr("JSON files (*.json);;XML files (*.xml)"),
                                                nullptr,
                                                QFileDialog::DontUseNativeDialog);
    if (QFileInfo(QFile(path)).suffix() != "xml" || QFileInfo(QFile(path)).suffix() != "xml") {
      path.append(".json");
    }
    *file_path = path;
    emit saveSingleFileAs(path);
#else
    QString path = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                         QStandardPaths::writableLocation(
                                                 QStandardPaths::DocumentsLocation)
                                         + "/Chart Creator Projects",
                                         tr("JSON files (*.json);;XML files (*.xml)"));
    if(!path.endsWith(".xml") || !path.endsWith(".json")) {
        path.append(".json");
    }
    *file_path = path;
    emit saveSingleFileAs(path);
#endif
  }
}

void SingleFileView::enableDarkMode() {
  if (tool_bar->actions().contains(dark_mode_action)) {
    tool_bar->removeAction(dark_mode_action);
    tool_bar->addAction(light_mode_action);
  }

  chart_view->darkMode();
  light_mode = false;
}

void SingleFileView::enableLightMode() {
  if (tool_bar->actions().contains(light_mode_action)) {
    tool_bar->removeAction(light_mode_action);
    tool_bar->addAction(dark_mode_action);
  }

  chart_view->lightMode();
  light_mode = true;
}
