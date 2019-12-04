#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#include<iostream>
#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 5;
    shape = 0;
    myPenColor = Qt::black;
}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loadedImage;

    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setShape(int newShape)
{
    shape = newShape;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;

        beginingPoint = event->pos();
    }
}


void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling){
        if(shape == 0){
            drawLineTo(event->pos());
        }
        //        if(shape == 1){
        //            //drawRectangle(event->pos());
        //            //beginingPoint = event->pos();
        //        }
    }

}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        if(shape == 0){
            this->setCursor(Qt::ArrowCursor);
            drawLineTo(event->pos());
            scribbling = false;
        }

        if(shape == 1){
            this->setCursor(Qt::UpArrowCursor);
            drawRectangle(event->pos());
            scribbling = false;
        }

        if(shape == 2){
            this->setCursor(Qt::UpArrowCursor);
            drawCircle(event->pos());
            scribbling = false;
        }

        if(shape == 3){
            this->setCursor(Qt::CrossCursor);
            drawStraigthLineTo(event->pos());
            scribbling = false;
        }

        if(shape == 4){
            this->setCursor(Qt::CrossCursor);
            fillShape(event->pos());
            scribbling = false;
        }
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect dirtyRect = event->rect();

    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    setPenWidth(5);
    painter.drawLine(lastPoint, endPoint);

    modified = true;

    int rad = (myPenWidth / 1) + 1;

    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;

}

void ScribbleArea::drawStraigthLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    qInfo() << endPoint;
    int x1 = beginingPoint.x();
    int y1 = beginingPoint.y();
    int x2 = endPoint.x();
    int y2 = endPoint.y();

    painter.drawLine(x1,y1,x2,y2);

    modified = true;

    int rad = (myPenWidth / 1) + 1;

    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}


int ScribbleArea::fill(int x, int y, QColor color){
     //qInfo() <<"b";
     // qInfo() <<x;
     // qInfo() <<y;
     //qInfo() << color;
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

//    if((x<=-1) || (x>=image.width()) || (y<=-1) || (y>image.height()-20)){
//        return 0;
//    }

    if((x<=-1) || (x>=500) || (y<=-1) || (y>=480)){
        return 1;
    }
    QColor colorPixelcheck = image.pixelColor(x,y);
    //qInfo() <<"a";
   // qInfo() << colorPixelcheck;

    if(colorPixelcheck != color){
       // qInfo() << "test1";
        return 1;
    }

    if(colorPixelcheck == color){
        //qInfo() << "test";
        painter.drawPoint(x,y);
        update();
        painter.end();
        //qInfo() << "test_color:";
        //qInfo() << image.pixelColor(x,y);
//        fill(x-1,y-1, color);
          fill(x,y-1, color);
//        fill(x+1,y-1, color);
          fill(x-1,y, color);
          fill(x+1,y, color);
//        fill(x-1,y+1, color);
          fill(x,y+1, color);
//        fill(x+1,y+1, color);
    }

    return 1;

}

void ScribbleArea::fillShape(const QPoint &endPoint)
{

    //qInfo() << endPoint;
    qInfo() << image.size();
    qInfo() << image.width();
    qInfo() << image.height();

    int x1 = endPoint.x();
    int y1 = endPoint.y();
    setPenWidth(1);
    recursioncolor = image.pixelColor(x1,y1);
    fill(x1, y1, recursioncolor);

    return;

}



void ScribbleArea::drawRectangle(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    //painter.drawLine(lastPoint, endPoint);
    qInfo() << endPoint;
    int x = beginingPoint.x();
    int y = beginingPoint.y();
    int width = endPoint.x()-x;
    int height= endPoint.y()-y;
    painter.drawRect(QRect(x,y,width,height));

    modified = true;

    int rad = (myPenWidth / 1) + 1;

    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void ScribbleArea::drawCircle(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    //painter.drawLine(lastPoint, endPoint);
    qInfo() << endPoint;
    int x = beginingPoint.x();
    int y = beginingPoint.y();
    int width = endPoint.x()-x;
    int height= endPoint.y()-y;
    painter.drawEllipse(QRect(x,y,width,height));

    modified = true;

    int rad = (myPenWidth / 4) + 4;

    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


void ScribbleArea::print()
{

#if QT_CONFIG(printdialog)


    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
