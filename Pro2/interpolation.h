#ifndef FFD_H
#define FFD_H
#include<QPainter>
#include "complexor.h"
//Interpolation Class
class Interpolation
{
public:
    Interpolation(QVector<QPointF> points,int num, int density);
    QVector<QPointF> mStartPoints;//起始图形的顶点数组
    QVector<QPointF> mEndPoints;//终止图形的顶点数组
    Complexor * pointComplexors;//保存向量和极坐标信息
    int mNum;//一个图形中点的数目
    int mDensity;//
    //获得插值点数组
    QPointF* getLinear();
    QPointF* getVectorLinear();
    QPointF* getClockVectorLinear();
    QPointF* getAntiClockVectorLinear();
    virtual ~Interpolation();

private:
    QPointF *mLinearKnots;//线性插值控制点数组
    QPointF *mVectorKnots;//向量线性插值控制点数组
    QPointF *mClockVectorKnots;//顺时针向量线性插值控制点数组
    QPointF *mAntiClockVectorKnots;//逆时针向量线性插值控制点数组
    //产生插值点
    void setLinear();
    void setVectorLinear();
};
#endif // FFD_H
