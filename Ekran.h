#ifndef EKRAN_H
#define EKRAN_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    QImage canvas;
protected:
    void paintEvent(QPaintEvent* event);
    // void mouseMoveEvent(QMouseEvent* event);
    void drawLine(int x1, int y1, int x2, int y2, uint red, uint green, uint blue);


signals:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // EKRAN_H
