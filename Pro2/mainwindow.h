#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include<QPixmapCache>
#include <QPropertyAnimation>
#include<QPainter>
#include<QMouseEvent>
#include<QTime>
#include<QTimer>
#include "interpolation.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int flag;
    bool canPlay;
    bool isGen;
    bool isPlay;
    int tick;
    QVector<QPointF> mPoints;//鼠标输入的点
    QVector<QPointF> mStartP;//起始图形的控制点
    QVector<QPointF> mEndP;//终止图形的控制点
    QVector<QPointF> mHandPoints;
    QVector<QPointF> mPenPoints;
    QVector<int> mPointNum;
    int tempPointNum;
    int mDrawTimes;
    Interpolation * mInterpolation;
    QPointF *Knots;//插值点数组
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void clearAll();
    ~MainWindow();

private slots:

    void on_End_Draw_clicked();
    void on_Play_clicked();
    void on_Clear_clicked();
    void timerInterpolation();
    void on_Hand_clicked();
    void on_Pen_clicked();

private:
    QTimer * timer;
    Ui::MainWindow *ui;
    void genHand();
    void genPen();
};

#endif // MAINWINDOW_H
