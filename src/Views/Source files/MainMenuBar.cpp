#include "../Header files/MainMenuBar.h"

MainMenuBar::MainMenuBar(QWidget *parent) {
    setParent(parent);

    file_menu = new QMenu("File", this);
    addMenu(file_menu);
}