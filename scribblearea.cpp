#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include <QPoint>
#endif
#endif
#include<iostream>
#include <queue>
#include <vector>
#include "scribblearea.h"


///Obiekt dziedziczący po QWidget, w środku ustalane są parametry początkowe
ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    myScriWidth=500;
    myScriHeight=600;
    modified = false;
    scribbling = false;
    myPenWidth = 5;
    shape = 0;
    myPenColor = Qt::black;
}
///Funckja odpowiedzialna za otwiernia pliku, na wejściu przyjmuje nazwe pliku, zwraca true albo false, jeżeli nie udało sie wczytać pliku o podanej nazwie zwraca false. Dodatkowo funkja dostosowuje rozmiar płótna do obrazu
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
///Funkja odpowiedzialna za zapisanie obrazu. Przyjmuje nazwe pliku i format, zwraca true albo false.
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
///Funkja odpowiedzialna za ustawienie koluru pena.
void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}
///Funkja odpowiedzialna za ustawienie kształtu pena.
void ScribbleArea::setShape(int newShape)
{
    shape = newShape;
}
///Funkja odpowiedzialna za ustawienie rozmiaru pena.
void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}
///Funkja odpowiedzialna za czyszczenie płótna zaznacza ze obraz był modyfikowany.
void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
///Funkja odpowiedzialna obsługe zdarzenia kliknięcia lpm. Zapisuje w beginingPoint oraz w lastPoint pozycje gdzie sie znajduje kursor.
void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;

        beginingPoint = event->pos();
    }
}

///Funkja odpowiedzialna obsługe zdarzenia przesunięcia myszki z wcisnietym lpm. Mamy tu 2 przypadki 1. jest narzedzie pióro, a 2. gumka. Przekazuje pocyje w którym ma miejsce zdażenie.
void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling){
        if(shape == 0){
            drawLineTo(event->pos());
        }
        if(shape == 5){
            QColor SaveC =penColor();
            int SSize =penWidth();
            setPenWidth(15);
            setPenColor(Qt::white);
            drawLineTo(event->pos());
            setPenColor(SaveC);
            setPenWidth(SSize);
        }
    }

}
///Funkcja odpowiedzialna za zwolnienie obsługe zdarzenia zwolnienia przycisku myszki, zwolnienie lewego przycisku myszki ustawia scribbling na false wiec dalszy ruch myszki nie bedzie rejestrowany, dodatkowo do funksji przkazywany jest pixel na którym był kursor przy zwolnieniu myszki co okresla jakiej wielkości ma byc rysowany obiekt
void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        if(shape == 0){
            drawLineTo(event->pos());
            scribbling = false;
        }

        if(shape == 1){

            drawRectangle(event->pos());
            scribbling = false;
        }

        if(shape == 2){

            drawCircle(event->pos());
            scribbling = false;
        }

        if(shape == 3){
            drawStraigthLineTo(event->pos());
            scribbling = false;
        }

        if(shape == 4){
            QColor SaveC =penColor();
            int SSize =penWidth();
            fillShape(event->pos());
            scribbling = false;
            setPenColor(SaveC);
            setPenWidth(SSize);
        }

    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect dirtyRect = event->rect();

    painter.drawImage(dirtyRect, image, dirtyRect);
}
///Funckja która obsługuje zdarzenie zmiany rozmiaru okna, nic nie zwraca, w przypadku zdarzenia albo dodaje do szerokości i długosci 4 pixeli bądź zmienia rozmiar płutna do rozmiaru image.
void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 4, image.width());
        int newHeight = qMax(height() + 4, image.height());
        myScriWidth=newWidth;
        myScriHeight=newHeight;
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
///Funckja polegająca na rysowaniu "lini" z punktu począatkowego do podanego jako argument punktu końcowego, jako ze obsługuje narzędzia pendzlopodbne w praktyce z kazdym ruchem myszki z wciśniętym lpm rysuje sie kropka o rozmiarach pedzla. W przypadku rysowania prostych lini działa domyslnie.
void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));


    painter.drawLine(lastPoint, endPoint);

    modified = true;

    int rad = (myPenWidth / 1) + 1;

    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;

}
///Funckja polegająca na rysowaniu prostej lini z punktu początkowego do podanego jako argument, punktu końcowego i wykkonuje funkcje drawLine(). Ustawia zmienną modified na true. Oraz odświerza płótno.
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



///Funkcja wypełniające przestrzenie, działą iteracyjnie, poniewaz werja algorytmu rekurencyjna jest niepraktyczna ze względu na przepełnienie stosu. Ustawia pixel początkowy do kolejki "do pomalowania" gdy ma inny kolor niz rządany, następnie go koloruje i sprawdza pixele w 4 strony góra,prawo, dół i lewo. Jeżeli ich kolor jest inny niz pożądany dodaje je na kolejke. Pętla bedzie się wykonywac dopóki kolejka nie będzie pusta.
void ScribbleArea::fill(int x, int y, QColor color) {
    QPoint source = QPoint(x,y);
    QColor sourceColor = image.pixelColor(source);
    if (sourceColor == color) {
            return ;
        }
    image.setPixel(source, color.rgb());

    std::queue<QPoint> pixelsQueue;
    pixelsQueue.push(source);

    while (!pixelsQueue.empty() ) {
        QPoint current_pixel = pixelsQueue.front();
        pixelsQueue.pop();


        if (sourceColor == image.pixelColor(current_pixel.x()+1,current_pixel.y())) {
            QPoint pixel_to_color = QPoint(current_pixel.x()+1, current_pixel.y());
            image.setPixel(pixel_to_color, color.rgb());
            pixelsQueue.push(pixel_to_color);
        }
        if (sourceColor == image.pixelColor(current_pixel.x()-1,current_pixel.y())){
            QPoint pixel_to_color = QPoint(current_pixel.x()-1, current_pixel.y());
            image.setPixel(pixel_to_color, color.rgb());
            pixelsQueue.push(pixel_to_color);
        }
        if (sourceColor == image.pixelColor(current_pixel.x(),current_pixel.y()+1)){
            QPoint pixel_to_color = QPoint(current_pixel.x(),current_pixel.y()+1);
            image.setPixel(pixel_to_color, color.rgb());
            pixelsQueue.push(pixel_to_color);
        }
        if (sourceColor == image.pixelColor(current_pixel.x(),current_pixel.y()-1)){
            QPoint pixel_to_color = QPoint(current_pixel.x(),current_pixel.y()-1);
            image.setPixel(pixel_to_color, color.rgb());
            pixelsQueue.push(pixel_to_color);
        }
    }
    update();
}
///Funkcja pomocnicza do wypełniania
void ScribbleArea::fillShape(const QPoint &endPoint)
{
    int x1 = endPoint.x();
    int y1 = endPoint.y();
    setPenWidth(1);
    fill(x1, y1, myPenColor);

    return;

}

///Funkcja rysujaca kwadrat, posiadając dwa przeciwległe położenia wierzchołków mozna narysowac kwadrat. Uzywamy do tego gotowej funkcji drawRect, jednak przed obliczamy wysokośc i długość z punktów.
void ScribbleArea::drawRectangle(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));

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
///Funkcja używana do rysowania elipsy, ptrzebujemy tyle samo informacji co w kwadracie ponieważ rysujemy elipse która jest w niego wpisana. na koniec odświeżamy płótno
void ScribbleArea::drawCircle(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    int x = beginingPoint.x();
    int y = beginingPoint.y();
    int width = endPoint.x()-x;
    int height= endPoint.y()-y;
    painter.drawEllipse(QRect(x,y,width,height));

    modified = true;

    int rad = (myPenWidth / 4) + 4;

    update();

    lastPoint = endPoint;
}
///Funckja zmieniające rozmiar image na podany w argumentach. Dodatkowo przyjmuje wskaźnik na image. Nowy obszar wypełniany jest białym kolorem. Następnie w pixelu 0,0 rysowany jest image.
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

///Funkcja odpowiedzialna za drukowanie. Wiekszosc jest przygotowana przez framework Qt, należy podać tylko odpowiednie dane do funkcji QPrint.
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
