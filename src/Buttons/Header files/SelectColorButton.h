#ifndef PROGETTOPAO_SELECTCOLORBUTTON_H
#define PROGETTOPAO_SELECTCOLORBUTTON_H
#include "QPushButton"
#include "QColor"
#include "QColorDialog"
#include "QDebug"


class SelectColorButton: public QPushButton {
    Q_OBJECT
private:
    QColor color;
public:
    explicit SelectColorButton(QWidget *parent = nullptr);
    QString getColor() const;
    void updateColorFromModel(const QString& new_color);
public slots:
    void updateBackgroundColor();
    void changeColor();
    void setColor(const QString& new_color);
    signals:
    void colorChanged(const QString& new_color);
};


#endif //PROGETTOPAO_SELECTCOLORBUTTON_H
