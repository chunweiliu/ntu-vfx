#include "widget.h"
#include "ui_widget.h"
#include "QFileDialog.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    // connect(ui->pushButton, SIGNAL(click()),this, SLOT(slotOpenImage()));

/*
    // set tarlabel and srclabel to black
    QPixmap initpixmap;
    QImage initimg;

    int labelSrcw = ui->labelSrcimg->width();
    int labelSrch = ui->labelSrcimg->height();
    int labelTarw = ui->labelTarimg->width();
    int labelTarh = ui->labelTarimg->height();

    initimg = QImage(labelSrcw, labelSrch, QImage::Format_RGB32);
    initimg.fill(0);
    initpixmap.fromImage(initimg);
    ui->labelSrcimg->setPixmap(initpixmap);

    initimg = QImage(labelTarw, labelTarh, QImage::Format_RGB32);
    initimg.fill(0);
    initpixmap.fromImage(initimg);
    ui->labelTarimg->setPixmap(initpixmap);

    update();
*/
    // initpixmap.fill();
    // ui->labelSrcimg->setPixmap(initpixmap);
    // ui->labelTarimg->setPixmap(initpixmap);
/*
    QImage qimg(ui->labelSrcimg->pixmap()->width(), ui->labelSrcimg->pixmap()->height(), QImage::Format_RGB888);
    qimg.fill(0);
    QPixmap qpixmap;
    qpixmap.fromImage(qimg);
    ui->labelSrcimg->setPixmap(qpixmap);
*/
}

Widget::~Widget()
{
    delete ui;
}
/*
void Widget::slotOpenImage( )
{

    QFileDialog file_dialog( this, tr( "Open Image" ), "./", tr( "PNG image (*.png)" ) );
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    QPixmap pixmap;
    pixmap.load( filenames.front() );

    ui->label->setPixmap( pixmap );

}
*/
/*
void Widget::on_pushButton_clicked()
{
    QFileDialog file_dialog( this, tr( "Open Image" ), "./", tr( "PNG image (*.png)" ) );
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    QPixmap pixmap;
    pixmap.load( filenames.front() );

    // ui->label->setPixmap( pixmap );
}

void Widget::on_pushButton_2_clicked()
{
       QFileDialog file_dialog( this, tr( "Open Image" ), "./", tr( "JPG image (*.jpg)" ) );
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    QPixmap pixmap;
    pixmap.load( filenames.front() );

    // ui->label->setPixmap( pixmap );
}
*/

void Widget::on_pushButtonSrcload_clicked()
{
    QFileDialog file_dialog( this, tr( "Open Image" ), "./", tr( "JPG image (*.jpg)" ) );
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    QPixmap pixmap;
    pixmap.load( filenames.front() );

    ui->labelSrcimg->setPixmap( pixmap );
}

void Widget::on_pushButtonTarload_clicked()
{
    QFileDialog file_dialog( this, tr( "Open Image" ), "./", tr( "JPG image (*.jpg)" ) );
    file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    QPixmap pixmap;
    pixmap.load( filenames.front() );

    ui->labelTarimg->setPixmap( pixmap );
}

void Widget::on_pushButtonSave_clicked()
{
    QFileDialog file_dialog( this, tr( "Save Image" ), "./", tr( "JPG image (*.jpg)" ) );
    // file_dialog.setFileMode( QFileDialog::ExistingFile );

    QStringList filenames;
    if ( file_dialog.exec() == QDialog::Accepted )
    {
        filenames = file_dialog.selectedFiles();
    }
    else
        return;

    const QPixmap& nowpixmap = *(ui->labelTarimg->pixmap());
    !nowpixmap.save(filenames.front(), "JPG");

}


void Widget::on_pushButtonClone_clicked()
{
}

void Widget::on_pushButton_clicked()
{

}

void Widget::on_labelSrcimg_linkActivated(QString link)
{

}
