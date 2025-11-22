#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "personmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_addPersonButton_clicked();

    void on_removePersonButton_clicked();

private:
    Ui::Widget *ui;

    PersonModel *m_model{};
};
#endif // WIDGET_H
