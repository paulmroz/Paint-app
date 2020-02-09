#include "mainwindow.h"
#include "scribblearea.h"
#include <QtWidgets>

///Tworzymy klase która dziedziczy z klasy Qt MainWindow o tej samej nazwie. Będzie ona naszym najwazniejszym oknem ze wzgledu na typ naszego programu.
MainWindow::MainWindow()
{
    ///Tworzymy nowe płótno(przestrzeń do rysowania).
    scribbleArea = new ScribbleArea;
    ///Ustawiamy aby płótno(przestrzeń do rysowania) była w centarlnym miejscu programu.
    setCentralWidget(scribbleArea);
    ///Uruchamiamy funkcjie odpowiedzialną za tworzenie akcji.
    createActions();
    ///Uruchamiamy funkcjie odpowiedzialną za tworzenie menu.
    createMenus();
    ///Ustawiamy tytuł na Paint.
    setWindowTitle(tr("Paint"));
    ///Ustawiamy ikone dla okna z zasobów.
    setWindowIcon(QIcon(":/images/icon.png"));
    ///Ustawiamy rozmiar początkowy okna na 500 na 600 pixeli.
    resize(500, 600);
    ///Dodajemy do Vektora mapypixeli ustawiony rozmiar okna.
    pixmapListe.push_back(new QPixmap(500,600));
    ///Przypisujemy int pixActuel rozmiar vektora pixmapListe pomniejszony o 1.
    pixActuel = pixmapListe.size()-1;
    ///Vektor pixelmapy o indexie 0 wypełniamy;
    pixmapListe[pixActuel]->fill();
}
///Funkcja opisuje działanie programu w przypadku pruby zamknięcia go. W naszym przypadku wyświetli sie komunikat czy na pewno chcemy wyjśc gdy mamy niezapisana prace. MainWindow jest tu przestrznia nazw. Funkcja nic nie zwraca.
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
///Funkcja obsługująca otwieranie pliku, jednak w przypadku gdy juz dokonalismy zmian na płótnie wywoła maybeSave() by zapobiec utracie ewentualnych postepów. Tworzy okno dialogowe gdzie podaje sie sciezke dostepu do pliku.
void MainWindow::open()
{
    if (maybeSave()) {

        QString fileName = QFileDialog::getOpenFileName(this,tr("Otwórz plik"), QDir::currentPath());

        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}
///Funkcja odpowiedzialna za zapisanie płótna do pliku przez kastowanie obiektów otrzymujemy format pliku graficznego wybranego przez uzytkownika z podmenu.
void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());

    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}
///Funkcja wywołuje panel wyboru kolory który jest przygotowany przez środowisko, nastepnie zapisuje wybrany kolor w zmiennej newColor i jeśli jest on poprawny zmienia kolor "Pena".
void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());

    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 5, w ten sposób rozróżnia narzędzie gumki.
void MainWindow::rubber()
{
        scribbleArea->setCursor(Qt::CrossCursor);
        scribbleArea->setShape(5);

}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 1, w ten sposób rozróżnia narzędzie do rysowania kwadratów.
void MainWindow::rectangle()
{
        scribbleArea->setCursor(Qt::UpArrowCursor);
        scribbleArea->setShape(1);
}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 0, w ten sposób rozróżnia narzędzie pędzel.
void MainWindow::line()
{
        scribbleArea->setCursor(Qt::ArrowCursor);
        scribbleArea->setShape(0);

}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 2, w ten sposób rozróżnia narzędzie do rysowania obwodów.
void MainWindow::circle()
{
        scribbleArea->setCursor(Qt::UpArrowCursor);
        scribbleArea->setShape(2);

}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 3, w ten sposób rozróżnia narzędzie do rysowania prostych lini.
void MainWindow::straightLine()
{
        scribbleArea->setCursor(Qt::CrossCursor);
        scribbleArea->setShape(3);
}
///Funkcja wykonuje inną funkcje w scribblearea.cpp przekazując jej parametr 4, w ten sposób rozróżnia narzędzie do wypełniania.
void MainWindow::filler()
{
        scribbleArea->setCursor(Qt::CrossCursor);
        scribbleArea->setShape(4);
}
///Funkcja obsługuje zmiane wielkości pędzla. Wyświetla okno dialogowe gdzie uzytkownik moze ustawic wartośc od 1 do 50, gdy wszytsko poszło pomyślnie ustawia ten rozmiar dla pendzla.
void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"),tr("Wybierz grubość pędzla:"), scribbleArea->penWidth(), 1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}


///Funkcja łączy sygnały ze slotami oraz dodaje obsługe skrótami klawiszowymi
void MainWindow::createActions()
{
    openAct = new QAction(tr("Otwórz..."), this);

    openAct->setShortcuts(QKeySequence::Open);

    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsActs.append(action);
    }


    printAct = new QAction(tr("&Drukuj..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));


    exitAct = new QAction(tr("Wyjdź"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Wybierz kolor..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Wybierz grubość..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    penRubberAct = new QAction(tr("Gumka..."), this);
    connect(penRubberAct, SIGNAL(triggered()), this, SLOT(rubber()));

    penLineAct = new QAction(tr("Pędzel..."), this);
    connect(penLineAct, SIGNAL(triggered()), this, SLOT(line()));

    penRectangleAct = new QAction(tr("Kwadrat..."), this);
    connect(penRectangleAct, SIGNAL(triggered()), this, SLOT(rectangle()));

    penCircleAct = new QAction(tr("Koło..."), this);
    connect(penCircleAct, SIGNAL(triggered()), this, SLOT(circle()));

    penStraightLineAct = new QAction(tr("Prosta Linia..."), this);
    connect(penStraightLineAct, SIGNAL(triggered()), this, SLOT(straightLine()));

    fillerAct = new QAction(tr("Wypełnianie"), this);
    connect(fillerAct, SIGNAL(triggered()), this, SLOT(filler()));

    clearScreenAct = new QAction(tr("&Wyczyść"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),scribbleArea, SLOT(clearImage()));
}
///Funkcja tworzy menu, wszystkie jego opcje oraz przypisuje danej opcji na menu, odpowiednie akcje
void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Zapisz jako"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&Plik"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Opcje"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(penRubberAct);
    optionMenu->addAction(penLineAct);
    optionMenu->addAction(penRectangleAct);
    optionMenu->addAction(penCircleAct);
    optionMenu->addAction(penStraightLineAct);
    optionMenu->addAction(fillerAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
}
///Funkcja obsługuje komunikat pojawiający się w przypadku, gdy użytkownik chce zamknąć prace bez zapisu, kożysta z funkcji isModified() płótna, gdy dojdzie do tego wyświetla komunikat w którym użytkownik moze wybrac opcje zapisu lub wyjśc pomimo ostrzeżeń
bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;

       ret = QMessageBox::warning(this, tr("Paint"),tr("Obraz został zmodyfikowany\n""Czy chcesz zapisać zmiany?."),QMessageBox::Save | QMessageBox::Discard| QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");

        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}
/// Funckja odpowiadająca za zapis pliku pliku do wskazanej przez użytkownika lokalizacji, dodatkowo wyśiwtela okno w której można wybrać lokalizacje za pomocą ekploatora oraz nazwe pliku, format danych jest wybrany z podmenu opcji
bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz jako"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}




