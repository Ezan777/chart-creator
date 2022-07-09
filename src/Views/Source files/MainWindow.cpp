#include <QJsonDocument>
#include <QCoreApplication>
#include "../Header files/MainWindow.h"

MainWindow::MainWindow() {
  data_tab = new CurrentFilesView(this);

  main_menu_bar = new MainMenuBar(this);

  setCentralWidget(data_tab);
  setMenuBar(main_menu_bar);

  QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  dir.mkdir("Chart Creator Projects");

  new_project_action = new QAction(tr("Create new project"));
  connect(new_project_action, &QAction::triggered, this, &MainWindow::createNewProject);
  main_menu_bar->file_menu->addAction(new_project_action);

  save = new QAction(tr("Save this"));
  connect(save, &QAction::triggered, data_tab, &CurrentFilesView::saveFileAtCurrentIndex);
  main_menu_bar->file_menu->addAction(save);

  save_all = new QAction(tr("Save all"));
  connect(save_all, &QAction::triggered, this, &MainWindow::saveAll);
  main_menu_bar->file_menu->addAction(save_all);

  load = new QAction(tr("Load project"));
  connect(load, &QAction::triggered, this, &MainWindow::loadFile);
  main_menu_bar->file_menu->addAction(load);

  load_from_riot = new QAction(tr("Load from Riot Games API"));
  connect(load_from_riot, &QAction::triggered, this, &MainWindow::createChartFromRiotApi);
  main_menu_bar->file_menu->addAction(load_from_riot);

  main_menu_bar->file_menu->insertSeparator(save);
  main_menu_bar->file_menu->insertSeparator(load);
  main_menu_bar->file_menu->insertSeparator(load_from_riot);

  connect(data_tab, &CurrentFilesView::noFileOpened, this, &MainWindow::openOrCreateDialog);
  connect(this, &MainWindow::quit, this, &QCoreApplication::quit, Qt::ConnectionType::QueuedConnection);

  setWindowTitle("Chart Creator");
  readSettings();
  show();
  data_tab->checkForFiles();
}

MainWindow::~MainWindow() {
  delete data_tab;
  delete main_menu_bar;
  delete new_project_action;
  delete save;
  delete save_all;
  delete load;
  delete load_from_riot;
}

void MainWindow::writeSettings() const {
  QSettings settings;

  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.endGroup();
}

void MainWindow::readSettings() {
  QSettings settings;

  settings.beginGroup("MainWindow");
  resize(settings.value("size", QSize(1100, 700)).toSize());
  move(settings.value("pos", QPoint(200, 200)).toPoint());
  settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  data_tab->saveSettings();
  delete data_tab;
  writeSettings();
  event->accept();
}

void MainWindow::createChartFromRiotApi() {
  QString summoner_name = QInputDialog::getText(this, tr("Insert the name of a summoner that plays in the EUW server"),
                                                tr("Summoner name"), QLineEdit::Normal,
                                                tr("Summoner name"));

  QJsonObject *last_ranked_info;
  try {
    last_ranked_info = RiotApi::lastMatchInfo(summoner_name);
  } catch (const QString& error) {
    QMessageBox error_message(QMessageBox::Icon::Warning,
                              tr("Warning"),
                              error,
                              QMessageBox::StandardButton::Ok,
                              this);
    error_message.exec();
    return;
  }
  QDateTime match_creation;
  match_creation.setTime_t((*last_ranked_info)["info"].toObject()["gameCreation"].toVariant().toLongLong() / 1000);
  QString date_of_the_match = match_creation.toString("dd-MM-yyyyTHH:mm");

  QJsonObject chart_ranked_info;
  chart_ranked_info["chartType"] = "BarChart";
  chart_ranked_info["chartTitle"] = "Players kda";

  QJsonArray colors_array;
  colors_array.append("#26c920");
  colors_array.append("#c91821");
  colors_array.append("#14a7d9");
  chart_ranked_info["colors"] = colors_array;

  QJsonArray labels_array;
  labels_array.append("Kills");
  labels_array.append("Deaths");
  labels_array.append("Assists");
  chart_ranked_info["labels"] = labels_array;

  QJsonArray series_array;
  QJsonArray data_array;
  QJsonArray participants = (*last_ranked_info)["info"].toObject()["participants"].toArray();
  for (const auto &participant : participants) {
    series_array.append(participant.toObject()["summonerName"].toString());
    data_array.append(QString::number(participant.toObject()["kills"].toInt()));
    data_array.append(QString::number(participant.toObject()["deaths"].toInt()));
    data_array.append(QString::number(participant.toObject()["assists"].toInt()));
  }

  chart_ranked_info["series"] = series_array;
  chart_ranked_info["data"] = data_array;

  QFile save_file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                      + "/Chart Creator Projects/" + "MatchOf" + date_of_the_match + ".json");

  save_file.open(QIODevice::WriteOnly);
  save_file.write(QJsonDocument(chart_ranked_info).toJson());
  save_file.close();

  QString file_path = QFileInfo(save_file).absoluteFilePath();
  data_tab->loadTab(&file_path);
  data_tab->setCurrentIndex(data_tab->count() - 1);
}

void MainWindow::openOrCreateDialog() {
  bool resolved = false;
  QMessageBox message_box;
  auto *create_button = new QPushButton(tr("Create"));
  auto *open_button = new QPushButton(tr("Open"));
  auto *exit_button = new QPushButton(tr("Exit"));

  message_box.setText(tr("There are no opened files"));
  message_box.setInformativeText(tr("What do you want to do?"));
  message_box.addButton(exit_button, QMessageBox::RejectRole);
  message_box.addButton(create_button, QMessageBox::AcceptRole);
  message_box.addButton(open_button, QMessageBox::AcceptRole);
  message_box.setDefaultButton(open_button);

  while (!resolved) {
    int result = message_box.exec();

    if (message_box.clickedButton() == nullptr || result == QDialog::Rejected) {
      resolved = true;
      emit quit();
    } else {
      if (message_box.clickedButton() == open_button) {
        resolved = loadFile();
      } else {
        resolved = createNewProject();
      }
    }
  }

  delete create_button;
  delete open_button;
}

void MainWindow::saveAll() const {
  data_tab->saveAllFiles();
}

bool MainWindow::loadFile() {
  /*I can't mark this function as const otherwise when I'll pass "this" to the
   * dialog constructor it will pass main window as const and the dialog
   * constructor does not accept that */
#ifdef __linux__
  QString file_path = QFileDialog::getOpenFileName(this, tr("Open a chart file"),
                                                   QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                                       + "/Chart Creator Projects",
                                                   tr("JSON files (*.json);;XML files (*.xml)"),
                                                   nullptr,
                                                   QFileDialog::DontUseNativeDialog);
#else
  QString file_path = QFileDialog::getOpenFileName(this, tr("Open a chart file"),
                                                   QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                                   +"/Chart Creator Projects",
                                                   tr("JSON files (*.json);;XML files (*.xml)"));
#endif
  if (file_path != "") {
    data_tab->loadTab(&file_path);
    data_tab->setCurrentIndex(data_tab->count() - 1);
    return true;
  } else {
    return false;
  }
}

bool MainWindow::createNewProject() {
  QStringList project_types;
  ChartType chart_type;
  project_types << tr("Bar chart") << tr("Pie chart") << tr("Spline chart");
  bool selected;
  QString type_selected = QInputDialog::getItem(this, tr("What type of chart do you want to create?"),
                                                tr("Project types"), project_types,
                                                0, false, &selected);

  if (selected && !type_selected.isEmpty()) {
    if (type_selected == "Bar chart") {
      chart_type = ChartType::BarChart;
    } else {
      if (type_selected == "Pie chart") {
        chart_type = ChartType::PieChart;
      } else {
        if (type_selected == "Spline chart") {
          chart_type = ChartType::SplineChart;
        }
      }
    }
  } else {
    return false;
  }

  QString file_name = QInputDialog::getText(this, tr("Insert a name for the project"),
                                            tr("Project's name"), QLineEdit::Normal,
                                            tr("My project"));
  if (file_name == "") {
    return false;
  }

  if (!file_name.endsWith(".xml") || !file_name.endsWith(".json")) {
    file_name.append(".json");
  }

#ifdef __linux__
  QString dir_path = QFileDialog::getExistingDirectory(this,
                                                       tr("Choose a location for your project"),
                                                       QStandardPaths::writableLocation(
                                                           QStandardPaths::DocumentsLocation)
                                                           + "/Chart Creator Projects",
                                                       QFileDialog::ShowDirsOnly |
                                                           QFileDialog::DontUseNativeDialog);
#else
  QString dir_path = QFileDialog::getExistingDirectory(this,
                                                                       tr("Choose a location for your project"),
                                                                       QStandardPaths::writableLocation(
                                                                               QStandardPaths::DocumentsLocation)
                                                                       + "/Chart Creator Projects",
                                                                       QFileDialog::ShowDirsOnly);
#endif
  if (dir_path != "") {
    QString file_path = dir_path.append("/" + file_name);
    QFile file(file_path);
    if (file.exists()) {
      QString suffix = QFileInfo(file).suffix();
      dir_path.remove(file_name);
      file_name.remove("." + suffix);
      file_name.append(tr("(copy)") + "." + suffix);
      file_path = dir_path.append(file_name);
      file.setFileName(file_path);

      if (!file.open(QFile::ReadOnly)) {
        QMessageBox error_message(QMessageBox::Icon::Critical,
                                  tr("Error"),
                                  tr("Unable to create the project"),
                                  QMessageBox::StandardButton::Ok,
                                  this);
        return false;
      } else {
        file.close();
      }
    } else {
      if (!file.open(QFile::ReadOnly)) {
        QErrorMessage error;
        error.showMessage(tr("Unable to create the new file"));
      } else {
        file.close();
      }
    }

    data_tab->createAnEmptyTab(file_path, chart_type);
    data_tab->setCurrentIndex(data_tab->count() - 1);
    data_tab->saveFileAtCurrentIndex();
    return true;
  } else {
    return false;
  }
}
