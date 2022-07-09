#ifndef PROGETTOPAO_SERIESVIEW_H
#define PROGETTOPAO_SERIESVIEW_H
#include "QWidget"
#include "QAbstractButton"
#include "QLabel"
#include "QBoxLayout"
#include "../../Containers/Header files/SeriesContainer.h"

class SeriesView: public QWidget {
    Q_OBJECT
private:
    AbstractModel *model;
    SeriesContainer *series_container;
    QPushButton *add_row;
    QPushButton *edit_button;
    QLabel *series_label;
    QVBoxLayout *layout;
    QHBoxLayout *buttons_layout;
public:
    explicit SeriesView(AbstractModel *chart_model, QWidget *parent = nullptr);
    ~SeriesView() override;
    void connectContainer() const;
public slots:
    void loadSeriesViewFromModel();
};


#endif //PROGETTOPAO_SERIESVIEW_H
