#ifndef PROGETTOPAO_MAINMENUBAR_H
#define PROGETTOPAO_MAINMENUBAR_H
#include "QAction"
#include "QMenuBar"
#include "QMenu"
#include "QIcon"

class MainMenuBar: public QMenuBar {
    Q_OBJECT
    friend class MainWindow;
public:
    MainMenuBar(QWidget* = nullptr);
private:
    QMenu *file_menu;
};


#endif //PROGETTOPAO_MAINMENUBAR_H
