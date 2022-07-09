#ifndef PROGETTOPAO_MAINWINDOW_H
#define PROGETTOPAO_MAINWINDOW_H
#include "QMainWindow"
#include "CurrentFilesView.h"
#include "MainMenuBar.h"
#include "QAction"
#include "QSettings"
#include "QFileDialog"
#include "QMessageBox"
#include "QInputDialog"
#include "QErrorMessage"
#include "../../HttpRequest/Header files/RiotApi.h"

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();
  ~MainWindow() override;
  CurrentFilesView *data_tab;
 private:
  MainMenuBar *main_menu_bar;
  QAction *new_project_action;
  QAction *save;
  QAction *save_all;
  QAction *load;
  QAction *load_from_riot;

  void writeSettings() const;
  void readSettings();

  void closeEvent(QCloseEvent *event) override;
 public slots:
  void createChartFromRiotApi();
  void openOrCreateDialog();
  void saveAll() const;
  bool loadFile();
  bool createNewProject();
 signals:
  void quit();
};

#endif //PROGETTOPAO_MAINWINDOW_H
