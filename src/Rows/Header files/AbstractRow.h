#ifndef PROGETTOPAO_ABSTRACTROW_H
#define PROGETTOPAO_ABSTRACTROW_H
#include "QWidget"
#include "QPushButton"
#include "QBoxLayout"
#include "QXmlStreamReader"
#include "QXmlStreamWriter"


class AbstractRow: public QWidget {
    Q_OBJECT
    friend class AbstractRowContainer;
public:
    QPushButton *delete_button;
    QHBoxLayout *layout;
public:
    explicit AbstractRow(QWidget *parent = nullptr);
    ~AbstractRow() override;
public slots:
    virtual void hideAllButtons() const;
    virtual void showAllButtons() const;
    virtual void loadRowFromModel() = 0;
    virtual void saveRow(QXmlStreamWriter *stream) const = 0;
    virtual void loadRow(QXmlStreamReader *stream) = 0;
    signals:
    void dataChanged();
};
#endif //PROGETTOPAO_ABSTRACTROW_H
