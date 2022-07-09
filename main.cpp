#include <QApplication>
#include <QResource>
#include "src/Views/Header files/MainWindow.h"
#include "QXmlStreamReader"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QResource::registerResource("../Data/resources.rcc");

    QCoreApplication::setApplicationName("Chart Creator");
    QCoreApplication::setOrganizationName("Zan");

    auto *main_window = new MainWindow;

    main_window->show();

    return QApplication::exec();
}
