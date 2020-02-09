#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

//Klasa odpowiedzialna za obsługe płótna, dziedziczy ona po klasie QWidget
class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = 0);
    ///Metoda służąca do otwierania obrazu, zwraca bool i przyjmuje wskaźnik na string, który jest nazwą pliku
    bool openImage(const QString &fileName);
    ///Metoda służąca do zapisu obrazu zwraca bool i przyjmuje wskaźnik na string, który jest nazwą pliku, oraz format plików któryj jest wskaźnikiem na char
    bool saveImage(const QString &fileName, const char *fileFormat);
    ///Metoda służąca ustawienia koloru pióra, przyjmuje kolor, który jest przkazany typem QColor
    void setPenColor(const QColor &newColor);
    ///Metoda służąca ustawienia wielkości pióra, przyjmuje rozmiar w liczbie od 1 do 50
    void setPenWidth(int newWidth);
    ///Metoda służąca ustawienia rodzaju narzędzia służącego do rysowania, przyjmuje liczbe od 1 do 5
    void setShape(int shape);
    ///Metoda służąca ustalenia czy płótno było modyfikowane
    bool isModified() const { return modified; }
    ///Metoda służąca do zwracania aktualnego koloru "pena"
    QColor penColor() const { return myPenColor; }
    ///Metoda służąca do zwracania aktualnego rozmiaru "pena"
    int penWidth() const { return myPenWidth; }

public slots:
    ///Obsługa sygnału czyszenie płótna
    void clearImage();
    ///Obsługa sygnału drukowania
    void print();

protected:
     ///Metoda służąca do obsługi zdarzenia kliknięcia  lpm
    void mousePressEvent(QMouseEvent *event) override;
    ///Metoda służąca do obsługi zdarzenia porószenia kursorem
    void mouseMoveEvent(QMouseEvent *event) override;
    ///Metoda służąca do obsługi zdarzenia zwolnienia  lpm
    void mouseReleaseEvent(QMouseEvent *event) override;
    ///Metoda służąca do obsługi zdarzenia drukowania
    void paintEvent(QPaintEvent *event) override;
    ///Metoda służąca do obsługi zdarzenia zmiany rozmiaru płótna
    void resizeEvent(QResizeEvent *event) override;

private:
    ///Metoda służąca do rysowania lini, narzędzie pióra,jako argument przyjmuje punkt gdzie linia ma sie kończyć
    void drawLineTo(const QPoint &endPoint);
    ///Metoda służąca do rysowania prostej lini, narzędzie rysowania prostej lini,jako argument przyjmuje gdzie linia ma sie kończyć
    void drawStraigthLineTo(const QPoint &endPoint);
    ///Metoda służąca do rysowania kwadratu, narzędzie rysowania kwadratu,jako argument przyjmuje gdzie kwadrat ma mieć przeciwny wierzchołek
    void drawRectangle(const QPoint &endPoint);
    ///Metoda służąca do rysowania okręgu, narzędzie rysowania okręgu,jako argument przyjmuje przeciwny wierzchołek kwadratu w który wpisane jest koło
    void drawCircle(const QPoint &endPoint);
    ///Metoda pomocnicza do wypełniajacej
    void fillShape(const QPoint &endPoint);
    ///Metoda służąca do zmiany rozmiaru płótna, przyjmuje wskaźnik na obraz, jego nowy rozmiar
    void resizeImage(QImage *image, const QSize &newSize);
    ///Metoda służąca do wypełniania powierzchni ograniczonych końcem płótna, bądź ograniczona przez inny kolor powierzchnie, narzędzie wypełniania
    void fill(int x,int y, QColor colorPixel);
    ///Zmienna przechowuje czy płótno było edytowane, gdy jest zapisywane jest ustawiana na false
    bool modified;
    ///Zmienna przechowuje czy właśnie dokonywane jest rysowanie
    bool scribbling;
    ///Zmienna przechowuje rozmiar pena
    int myPenWidth;
    ///Zmienna przechowuje kształt pena, czyli jakie narzędzie do rysowania zostało wybrane
    int shape;
    ///Zmienna przechowuje rozmiar płótna
    int myScriWidth;
    ///Zmienna przechowuje rozmiar płótna
    int myScriHeight;
    ///Zmienna przechowuje kolor pena
    QColor myPenColor;
    ///Zmienna przechowuje poprzedni kolor pena
    QColor myprevPenColor;
    QImage image;
    ///Zmienna przechowuje współrzędne końcowe
    QPoint lastPoint;
    ///Zmienna przechowuje współrzędne początkowe
    QPoint beginingPoint;

    QColor recursioncolor;

};

#endif
