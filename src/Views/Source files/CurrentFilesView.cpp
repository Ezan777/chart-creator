#include "../Header files/CurrentFilesView.h"

CurrentFilesView::CurrentFilesView(QWidget *parent) {
  setParent(parent);
  setTabsClosable(true);
  connect(this->tabBar(), &QTabBar::tabCloseRequested, this, &CurrentFilesView::closeTabAt);

  opened_files = new QList<SingleFileView *>;
  opened_files_paths = new QList<QString>;
  files_names = new QList<QString>;
  readSettings();

  if (opened_files_paths->length() == 0) {
    are_there_files = false;
  } else {
    are_there_files = true;
    for (int i = 0; i < opened_files_paths->length(); ++i) {
      QString file_path = opened_files_paths->at(i);
      QFile file(file_path);

      if (file.exists()) {
        opened_files->append(new SingleFileView(file_path, ChartType::BarChart, this));
        files_names->append(QString(QFileInfo(file_path).baseName()));
        opened_files->last()->loadSingleFile(&file_path);
        addTab(opened_files->last(), files_names->last());
        connect(opened_files->last(), &SingleFileView::saveSingleFileAs, this, &CurrentFilesView::saveSingleFile);
      } else {
        opened_files_paths->removeAll(file_path);
        --i;
        if (opened_files_paths->length() == 0) {
          are_there_files = false;
        }
      }

      QSettings settings;
      setCurrentIndex(settings.value("focus").toInt());
    }
  }

  show();
}

CurrentFilesView::~CurrentFilesView() {
  for (int i = 0; i < opened_files->length(); ++i) {
    delete opened_files->at(i);
  }
  delete opened_files;
  delete opened_files_paths;
  delete files_names;
}

void CurrentFilesView::saveSettings() const {
  QSettings settings;

  settings.setValue("focus", currentIndex());

  settings.beginWriteArray("OpenedFiles");
  for (int i = 0; i < opened_files_paths->length(); ++i) {
    settings.setArrayIndex(i);
    settings.setValue("path", opened_files_paths->at(i));
  }
  settings.endArray();
}

void CurrentFilesView::checkForFiles() {
  if (!are_there_files) {
    emit noFileOpened();
  }
}

void CurrentFilesView::readSettings() {
  QSettings settings;

  int size = settings.beginReadArray("OpenedFiles");
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    opened_files_paths->insert(i, settings.value("path").toString());
  }
  settings.endArray();
}

void CurrentFilesView::closeTabAt(int index) {
  opened_files->removeAt(index);
  opened_files_paths->removeAt(index);
  delete widget(index);

  if (count() == 0) {
    emit noFileOpened();
  }
}

void CurrentFilesView::saveAllFiles() const {
  for (int i = 0; i < opened_files->length(); ++i) {
    QFile file(opened_files_paths->at(i));
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("root");
    stream.writeStartElement("ChartCreatorProject");
    opened_files->at(i)->saveSingleFileToXML(&stream);
    stream.writeEndElement();
    stream.writeEndElement();
  }
}

void CurrentFilesView::loadTab(QString *file_path) {
  QFile file(*file_path);

  if (file.exists()) {
    opened_files->append(new SingleFileView(*file_path, ChartType::BarChart));
    opened_files_paths->append(*file_path);
    files_names->append(QString(QFileInfo(*file_path).baseName()));
    opened_files->last()->loadSingleFile(file_path);
    connect(opened_files->last(), &SingleFileView::saveSingleFileAs, this, &CurrentFilesView::saveSingleFile);
    addTab(opened_files->last(), files_names->last());
  }
}

void CurrentFilesView::createAnEmptyTab(const QString &path, ChartType chart_type) {
  opened_files->append(new SingleFileView(path, chart_type));
  connect(opened_files->last(), &SingleFileView::saveSingleFileAs, this, &CurrentFilesView::saveSingleFile);
  opened_files_paths->append(path);
  files_names->append(QString(QFileInfo(path).baseName()));

  addTab(opened_files->last(), files_names->last());
}

void CurrentFilesView::saveSingleFile(const QString &path) {
  if (path != opened_files_paths->at(currentIndex())) {
    opened_files_paths->replace(currentIndex(), path);
    QString new_name = QFileInfo(path).baseName();
    setTabText(currentIndex(), new_name);
    saveFileAtCurrentIndex();
  } else {
    saveFileAtCurrentIndex();
  }
}

void CurrentFilesView::saveFileAtCurrentIndex() const {
  QFile file(opened_files_paths->at(currentIndex()));
  file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QString suffix = QFileInfo(file).suffix();
  if (suffix == "xml") {
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("root");
    stream.writeStartElement("ChartCreatorProject");
    opened_files->at(currentIndex())->saveSingleFileToXML(&stream);
    stream.writeEndElement();
    stream.writeEndElement();
  } else {
    QJsonObject save_object;
    opened_files->at(currentIndex())->saveSingleFileToJson(&save_object);
    file.write(QJsonDocument(save_object).toJson());
  }
}
