/********************************************************************************
** Form generated from reading ui file 'widget.ui'
**
** Created: Sun Jun 28 21:42:50 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QProgressBar *progressBar;
    QSplitter *splitter_2;
    QLabel *labelSrcimg;
    QLabel *labelTarimg;
    QSplitter *splitter;
    QRadioButton *radioButtonAlgo1;
    QRadioButton *radioButtonAlgo2;
    QRadioButton *radioButtonAlgo3;
    QRadioButton *radioButtonAlgo4;
    QSplitter *splitter_3;
    QPushButton *pushButtonSrcload;
    QPushButton *pushButtonTarload;
    QPushButton *pushButtonClone;
    QPushButton *pushButtonSave;
    QSplitter *splitter_4;
    QSlider *horizontalSliderParam1;
    QSlider *horizontalSliderParam2;
    QLineEdit *lineEditParam3;
    QPushButton *pushButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(600, 400);
        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(130, 360, 461, 23));
        progressBar->setValue(100);
        progressBar->setInvertedAppearance(false);
        splitter_2 = new QSplitter(Widget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(130, 60, 441, 271));
        splitter_2->setOrientation(Qt::Horizontal);
        labelSrcimg = new QLabel(splitter_2);
        labelSrcimg->setObjectName(QString::fromUtf8("labelSrcimg"));
        labelSrcimg->setCursor(QCursor(Qt::OpenHandCursor));
        labelSrcimg->setAutoFillBackground(true);
        labelSrcimg->setPixmap(QPixmap(QString::fromUtf8("C:/Documents and Settings/sitrke/\346\241\214\351\235\242/resultmask.jpg")));
        labelSrcimg->setScaledContents(true);
        labelSrcimg->setAlignment(Qt::AlignCenter);
        splitter_2->addWidget(labelSrcimg);
        labelTarimg = new QLabel(splitter_2);
        labelTarimg->setObjectName(QString::fromUtf8("labelTarimg"));
        labelTarimg->setCursor(QCursor(Qt::OpenHandCursor));
        labelTarimg->setPixmap(QPixmap(QString::fromUtf8("C:/Documents and Settings/sitrke/\346\241\214\351\235\242/resultmask.jpg")));
        labelTarimg->setScaledContents(true);
        labelTarimg->setAlignment(Qt::AlignCenter);
        splitter_2->addWidget(labelTarimg);
        splitter = new QSplitter(Widget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(10, 60, 101, 271));
        splitter->setOrientation(Qt::Vertical);
        radioButtonAlgo1 = new QRadioButton(splitter);
        radioButtonAlgo1->setObjectName(QString::fromUtf8("radioButtonAlgo1"));
        splitter->addWidget(radioButtonAlgo1);
        radioButtonAlgo2 = new QRadioButton(splitter);
        radioButtonAlgo2->setObjectName(QString::fromUtf8("radioButtonAlgo2"));
        splitter->addWidget(radioButtonAlgo2);
        radioButtonAlgo3 = new QRadioButton(splitter);
        radioButtonAlgo3->setObjectName(QString::fromUtf8("radioButtonAlgo3"));
        splitter->addWidget(radioButtonAlgo3);
        radioButtonAlgo4 = new QRadioButton(splitter);
        radioButtonAlgo4->setObjectName(QString::fromUtf8("radioButtonAlgo4"));
        splitter->addWidget(radioButtonAlgo4);
        splitter_3 = new QSplitter(splitter);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        pushButtonSrcload = new QPushButton(splitter_3);
        pushButtonSrcload->setObjectName(QString::fromUtf8("pushButtonSrcload"));
        splitter_3->addWidget(pushButtonSrcload);
        pushButtonTarload = new QPushButton(splitter_3);
        pushButtonTarload->setObjectName(QString::fromUtf8("pushButtonTarload"));
        splitter_3->addWidget(pushButtonTarload);
        pushButtonClone = new QPushButton(splitter_3);
        pushButtonClone->setObjectName(QString::fromUtf8("pushButtonClone"));
        splitter_3->addWidget(pushButtonClone);
        pushButtonSave = new QPushButton(splitter_3);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        splitter_3->addWidget(pushButtonSave);
        splitter->addWidget(splitter_3);
        splitter_4 = new QSplitter(splitter);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        horizontalSliderParam1 = new QSlider(splitter_4);
        horizontalSliderParam1->setObjectName(QString::fromUtf8("horizontalSliderParam1"));
        horizontalSliderParam1->setMaximum(100);
        horizontalSliderParam1->setSingleStep(1);
        horizontalSliderParam1->setOrientation(Qt::Horizontal);
        splitter_4->addWidget(horizontalSliderParam1);
        horizontalSliderParam2 = new QSlider(splitter_4);
        horizontalSliderParam2->setObjectName(QString::fromUtf8("horizontalSliderParam2"));
        horizontalSliderParam2->setMaximum(100);
        horizontalSliderParam2->setOrientation(Qt::Horizontal);
        splitter_4->addWidget(horizontalSliderParam2);
        splitter->addWidget(splitter_4);
        lineEditParam3 = new QLineEdit(splitter);
        lineEditParam3->setObjectName(QString::fromUtf8("lineEditParam3"));
        splitter->addWidget(lineEditParam3);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 350, 75, 23));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        labelSrcimg->setText(QString());
        labelTarimg->setText(QString());
        radioButtonAlgo1->setText(QApplication::translate("Widget", "Algo 1", 0, QApplication::UnicodeUTF8));
        radioButtonAlgo2->setText(QApplication::translate("Widget", "Algo 2", 0, QApplication::UnicodeUTF8));
        radioButtonAlgo3->setText(QApplication::translate("Widget", "Algo 3", 0, QApplication::UnicodeUTF8));
        radioButtonAlgo4->setText(QApplication::translate("Widget", "Algo 4", 0, QApplication::UnicodeUTF8));
        pushButtonSrcload->setText(QApplication::translate("Widget", "source browser", 0, QApplication::UnicodeUTF8));
        pushButtonTarload->setText(QApplication::translate("Widget", "target browser", 0, QApplication::UnicodeUTF8));
        pushButtonClone->setText(QApplication::translate("Widget", "cloning", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QApplication::translate("Widget", "save result", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Widget", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Widget);
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
