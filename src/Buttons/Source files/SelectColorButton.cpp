#include "../Header files/SelectColorButton.h"

SelectColorButton::SelectColorButton(QWidget *parent): QPushButton(parent) {
    connect(this, &QPushButton::clicked, this, &SelectColorButton::changeColor);
}

QString SelectColorButton::getColor() const {
    return color.name();
}

void SelectColorButton::updateColorFromModel(const QString& new_color) {
    color.setNamedColor(new_color);
    updateBackgroundColor();
}

void SelectColorButton::updateBackgroundColor() {
    setStyleSheet("background-color: " + color.name());
}

void SelectColorButton::changeColor() {
    QColor new_color = QColorDialog::getColor(color, nullptr,
                                               tr("Choose a color"),
                                               QColorDialog::DontUseNativeDialog);
    if (new_color.isValid()) {
        setColor(new_color.name());
    }
}

void SelectColorButton::setColor(const QString &new_color) {
    color.setNamedColor(new_color);
    updateBackgroundColor();
    emit colorChanged(color.name());
}