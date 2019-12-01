#include <QtWidgets>
#include "mainwindow.h"
#include "scribblearea.h"


MainWindow::MainWindow()
{
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);
    createActions();
    createMenus();

    setWindowTitle(tr("Paint"));
    resize(500, 600);
     //resize(10, 20);
    pixmapListe.push_back(new QPixmap(500,600));
    pixActuel = pixmapListe.size()-1;
    pixmapListe[pixActuel]->fill();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave()) {

        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Otwórz plik"), QDir::currentPath());

        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());

    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());

    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

void MainWindow::rubber()
{
        scribbleArea->setShape(0);
        scribbleArea->setPenColor(Qt::white);
}

void MainWindow::rectangle()
{
        scribbleArea->setShape(1);
}

void MainWindow::line()
{
        scribbleArea->setShape(0);
}

void MainWindow::circle()
{
        scribbleArea->setShape(2);
}

void MainWindow::straightLine()
{
        scribbleArea->setShape(3);
}

void MainWindow::filler()
{
        scribbleArea->setShape(4);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"),
                                        tr("Wybierz grubość pędzla:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}



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
    connect(clearScreenAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));
}

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

bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;

       ret = QMessageBox::warning(this, tr("Paint"),
                          tr("Obraz został zmodyfikowany\n"
                             "Czy chcesz zapisać zmiany?."),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");

        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

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




