#include "mainwindow.h"
#include "scribblearea.h"
#include <QApplication>

///Główna programu wykonywana jako pierwsza, tworzymy tu obiekt a z klasy QApplication, oraz obiekt w z klasy MainWindow. Następnie za pomocą funkcji show() wyświetlamy okono, ostatnia linijka odpowiedzialna jest za wprowadzenie programu w petle, co umozliwi wyłapywanie zdarzeń.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
