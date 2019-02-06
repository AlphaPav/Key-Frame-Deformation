#ifndef COMPLEXOR_H
#define COMPLEXOR_H
#include<QPainter>
#include <QtCore/qmath.h>
class Complexor:public QObject
{
    Q_OBJECT
public:
    Complexor();
    float x;
    float y;
    float r;
    float angle;
    QPointF startPoint;
    QPointF endPoint;
    void setPolarByXY();
    void setEndPoint();
    virtual ~ Complexor();
};
#endif // COMPLEXOR_H
