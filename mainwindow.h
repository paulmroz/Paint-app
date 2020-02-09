#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QApplication>
#include <QtWidgets>
#include <QPoint>


class ScribbleArea;
///Tworzymy klase MainWindow która będzie naszym oknem, dziedziczy ona z QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ///Konstruktor naszego okna.
    MainWindow();

protected:
    ///Medota odpowiedzialna za kliknięcie "x" na pasku zadań.
    void closeEvent(QCloseEvent *event) override;


private slots:
      ///Element kompilowany przez moc. Slot jest funkcją, która jest wywoływana w odpowiedzi na wyemitowany sygnał. Jest to slot "open" odpowiedzialny za otwieranie plików. W tym przypadku sygnałem będzie kliknięcie przycisku z menu "Otwórz..."
    void open();
     ///Jest to slot "save" odpowiedzialny za zapis plików.
    void save();
      ///Jest to slot "penColor" odpowiedzialny za wywołanie przygotowanej palety do doboru kolorów.
    void penColor();
    ///Jest to slot "penWidth" odpowiedzialny za otorzenie okna wyboru grubości narzedzia.
    void penWidth();
    ///Jest to slot "rubber" odpowiedzialny za gumke.
    void rubber();
     ///Jest to slot "rectangle" odpowiedzialny za rysowanie kwadratu.
    void rectangle();
    ///Jest to slot "line" odpowiedzialny za rysowanie lini.
    void line();
    ///Jest to slot "circle" odpowiedzialny za rysowanie obwodu.
    void circle();
    ///Jest to slot "straightLine" odpowiedzialny za rysowanie koła.
    void straightLine();
    ///Jest to slot "filler" odpowiedzialny za narzędzie wypełniania.
    void filler();

private:
    ///Deklaracja funkcji tworzącej wszelakie akcje
    void createActions();
    ///Deklaracja funkcji umieszcające menu na głównego okna
    void createMenus();
    ///Deklaracja funkcji proponująca użytkownikowi zapisu w razie wyjścia
    bool maybeSave();
    ///Deklaracja funkcji uzywanej do zapisu pliku
    bool saveFile(const QByteArray &fileFormat);
    ///Najważniejszy elemet naszego programu czyli płótno jest opisane w widżecie ScribbleArea. Deklarujemy wskaźnik na obiekt o nazwie *scribbleArea z klasy ScribbleArea.
    ScribbleArea *scribbleArea;
    ///Tworzymy obiekty z klasy QMenu. w tym przypadku jest to podmenu w którym wybieramy typ pliku.
    QMenu *saveAsMenu;
    ///Tworzymy obiekty z klasy QMenu. w tym przypadku jest to menu odpowiedzialne za prace z plkikiem w programie.
    QMenu *fileMenu;
    ///Tworzymy obiekty z klasy QMenu. w tym przypadku jest to menu odpowiedzialne za wybór narzedzi, koloru czy rozmiaru pędzla.
    QMenu *optionMenu;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *openAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QList<QAction *> saveAsActs;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *exitAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penColorAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penLineAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penWidthAct;
    ///Deklarujemy wskaźnik n
    /// ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.a obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penRubberAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penRectangleAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penCircleAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *penStraightLineAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *printAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *fillerAct;
    ///Deklarujemy wskaźnik na obiekt tymu QAction. Jest to abstrakcyjny element obsługi interfejsu, który pozwala na odnoszenie sie do operacji z menu bądź toolbara.
    QAction *clearScreenAct;
    ///Deklarujemy inta który bedzie zawierał zozmiar Vektora pixelmapy.
    long pixActuel;
    ///Deklarujemy vektor który będzie służył za pixelmape.
    QVector<QPixmap*> pixmapListe;

};

#endif
