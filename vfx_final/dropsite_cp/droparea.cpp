/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <cassert>

#include "droparea.h"

//! [DropArea constructor]
DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}
//! [DropArea constructor]

//! [dragEnterEvent() function]
void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    emit changed(event->mimeData());
}
//! [dragEnterEvent() function]

//! [dragMoveEvent() function]
void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
//! [dragMoveEvent() function]

//! [dropEvent() function part1]
void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
//! [dropEvent() function part1]

//! [dropEvent() function part2]
    if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(Qt::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());    
        setTextFormat(Qt::PlainText);
    } else if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        //QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            //text += url + QString("\n");
            url.remove(0,1);
            text += url;
        }
        //setText(text);
        // show the image when get the position of image

        QPixmap pixmap;
        pixmap.load( text );
        setPixmap( pixmap );

        // Initial myBuffer
        myBuffer = new QPixmap;
        myBuffer->load(text);
        //QPixmap pixmap = *myBuffer;
        //setPixmap(pixmap);

    } else {
        setText(tr("Cannot display data"));
    }
//! [dropEvent() function part2]

//! [dropEvent() function part3]    
    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
}
//! [dropEvent() function part3]

//! [dragLeaveEvent() function]
void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}
//! [dragLeaveEvent() function]

//! [clear() function]
void DropArea::clear()
{
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Dark);

    emit changed();
}
//! [clear() function]

void DropArea::mousePressEvent(QMouseEvent *event)
{

    //startX = event->globalX() - this->x();
    //startY = event->globalY() - this->y();
    startX = event->x() - this->x();
    startY = event->y() - this->y();
    //setPixmap( *myBuffer );
    //debug->setText( tmp );
    QPixmap pixmap;
    pixmap.load(text);
    setPixmap(pixmap);

}


void DropArea::mouseMoveEvent( QMouseEvent *event )
{
    int endX, endY;
    //endX = event->globalX() - this->x();
    //endY = event->globalY() - this->y();
    endX = event->x() - this->x();
    endY = event->y() - this->y();
    this->redrawRect( startX, startY, endX, endY );

}

/*
void DropArea::mouseReleaseEvent(QMouseEvent *event)
{
    int endX, endY;
    //endX = event->globalX();
    //endY = event->globalY();
    endX = event->x();
    endY = event->y();
    int pixmapW = myBuffer->width();
    int pixmapH = myBuffer->height();

    QImage *mask = new QImage( pixmapW, pixmapH, QImage::Format_RGB32);

    QPixmap selectedRect;
    QPainter painter(&selectedRect);
    for( int y = startX; y < endY; ++y ) {
        for( int x = startY; x < endX; ++x ) {
            //mask->setPixel( QPoint( x, y ), 1 );
            //mask->setPixel( x, y , QRgba(0, 0, 0, 0) );
            //setPixmap(x, y, QRgb(0, 0, 0));
            // ... still don't know how to set a image pixel of a frame
            mask->setPixel( x, y, qRgb(255,255,0) );

            //rect.fill( qRgb(255, 255, 0));
            //setPixmap(x, y, qRgb(255,255,0) );

        }
    }


    emit getArea( mask );
}
*/

void DropArea::redrawRect( int startX, int startY, int endX, int endY )
{
    QPixmap qpic( myBuffer->width(), myBuffer->height() );

    QPainter *painter = new QPainter( &qpic );

    painter->drawPixmap( 0, 0,  *myBuffer );

    painter->setPen( QPen( QColor( 0, 255, 0, 255 ) ) );        // edge
    painter->setBrush( QBrush( QColor( 0, 255, 0, 128 ) ) );    // inside
    painter->drawRect( QRect( startX, startY, endX-startX, endY-startY ) );

    painter->end();
    setPixmap( qpic );
}

