#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ScribbleArea;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow();

protected:

    void closeEvent(QCloseEvent *event) override;


private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void rubber();
    void rectangle();
    void line();
    void circle();
    void straightLine();

private:

    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penLineAct;
    QAction *penWidthAct;
    QAction *penRubberAct;
    QAction *penRectangleAct;
    QAction *penCircleAct;
    QAction *penStraightLineAct;
    QAction *printAct;
    QAction *clearScreenAct;

};

#endif
