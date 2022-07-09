#ifndef PROGETTOPAO_CURRENTFILESVIEW_H
#define PROGETTOPAO_CURRENTFILESVIEW_H
#include "BarDataView.h"
#include "QTabWidget"
#include "QHBoxLayout"
#include "ChartView.h"
#include "../../Containers/Header files/SeriesContainer.h"
#include "SeriesView.h"
#include "QTabBar"
#include "QFileInfo"
#include "SingleFileView.h"
#include "QSettings"
#include "QDir"
#include "QStandardPaths"

class CurrentFilesView : public QTabWidget {
 Q_OBJECT
 public:
  explicit CurrentFilesView(QWidget *parent = nullptr);
  ~CurrentFilesView() override;
  void saveSettings() const;
  void checkForFiles();
 private:
  QList<SingleFileView *> *opened_files;
  QList<QString> *opened_files_paths;
  QList<QString> *files_names;
  QDir last_file_dir;
  bool are_there_files;

  void readSettings();
 public slots:
  void closeTabAt(int index);
  void saveAllFiles() const;
  void loadTab(QString *file_path);
  void createAnEmptyTab(const QString &path, ChartType chart_type);
  void saveSingleFile(const QString &path = nullptr);
  void saveFileAtCurrentIndex() const;
 signals:
  void noFileOpened();
};

#endif //PROGETTOPAO_CURRENTFILESVIEW_H
