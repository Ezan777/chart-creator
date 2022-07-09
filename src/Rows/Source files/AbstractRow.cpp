#include "../Header files/AbstractRow.h"

AbstractRow::AbstractRow(QWidget *parent) {
    setParent(parent);

    QIcon delete_icon(":/icons/delete_bin.png");
    delete_button = new QPushButton(this);
    delete_button->setIcon(delete_icon);
    delete_button->hide();

    layout = new QHBoxLayout(this);
    layout->addWidget(delete_button);
}

AbstractRow::~AbstractRow() {
    delete delete_button;
    delete layout;
}

void AbstractRow::hideAllButtons() const {
    delete_button->hide();
}

void AbstractRow::showAllButtons() const {
    delete_button->show();
}