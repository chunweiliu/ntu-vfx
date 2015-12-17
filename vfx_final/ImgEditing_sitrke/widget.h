#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>

namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
private slots:
    // void on_pushButton_2_clicked();
    // void on_pushButton_clicked();
    // void slotOpenImage();

private slots:
    void on_labelSrcimg_linkActivated(QString link);
    void on_pushButton_clicked();
    void on_pushButtonClone_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonTarload_clicked();
    void on_pushButtonSrcload_clicked();
};

#endif // WIDGET_H
