#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = 0);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setShape(int shape);
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void drawStraigthLineTo(const QPoint &endPoint);
    void drawRectangle(const QPoint &endPoint);
    void drawCircle(const QPoint &endPoint);
    void fillShape(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    int fill(int x,int y, QColor colorPixel);

    bool modified;
    bool scribbling;
    int myPenWidth;
    int shape;
    QColor myPenColor;
    QColor myprevPenColor;
    QImage image;
    QPoint lastPoint;
    QPoint beginingPoint;

    QColor recursioncolor;

};

#endif
