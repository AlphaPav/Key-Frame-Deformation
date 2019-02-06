#include "interpolation.h"
#include <qDebug>
Interpolation::Interpolation(QVector<QPointF> points,int num, int density)
{
    mNum=num;
    mDensity=density;
    int i;
    for (i=0; i< num;i++) mStartPoints.append(points.at(i));
    for(i= num; i< 2*num; i++) mEndPoints.append(points.at(i));
    mLinearKnots=NULL;
    mVectorKnots=NULL;
    setLinear();
    setVectorLinear();
}

void Interpolation::setLinear(){
    mLinearKnots= (QPointF*) malloc(sizeof(QPointF)*mNum*mDensity);
    int i,j;
    float x,y;
    for (i=0; i<mNum;i++) mLinearKnots[i]=mStartPoints.at(i);
    for(i=mNum*(mDensity-1); i<mNum*mDensity;i++ ) mLinearKnots[i]=mEndPoints.at(i-mNum*(mDensity-1));
    for(j=1;j<mDensity;j++)
    {
        float t= (float) j/(mDensity-1);
        for(i=0;i<mNum;i++)
        {
            x= (1-t)* mLinearKnots[i].x() + t* mLinearKnots[mNum*(mDensity-1)
                    +i].x();
            y=(1-t)* mLinearKnots[i].y() + t* mLinearKnots[mNum*(mDensity-1)
                    +i].y();
            mLinearKnots[j*mNum+i].setX(x);
            mLinearKnots[j*mNum+i].setY(y);
        }
    }
}
QPointF* Interpolation::getLinear()
{
    return mLinearKnots;
}
void Interpolation::setVectorLinear(){
    mVectorKnots= (QPointF*)malloc(sizeof(QPointF)*mNum*mDensity);
    mClockVectorKnots= (QPointF*)malloc(sizeof(QPointF)*mNum*mDensity);
    mAntiClockVectorKnots= (QPointF*)malloc(sizeof(QPointF)*mNum*mDensity);
    pointComplexors= (Complexor*)malloc(sizeof(Complexor)*(mNum-1)*mDensity);
    int i,j;
    float x,y,r,angle;

    for(i=1; i<=(mDensity-1);i++)//第一个点线性插值
    {
         float t= (float) i/(mDensity);
         x= (1-t)* mStartPoints[0].x() + t* mEndPoints[0].x();
         y= (1-t)* mStartPoints[0].y() + t* mEndPoints[0].y();
         mVectorKnots[i*mNum].setX(x);
         mVectorKnots[i*mNum].setY(y);
         mAntiClockVectorKnots[i*mNum].setX(x);
         mAntiClockVectorKnots[i*mNum].setY(y);
         mClockVectorKnots[i*mNum].setX(x);
         mClockVectorKnots[i*mNum].setY(y);
    }
    for (i=0; i<mNum;i++)
    {
        mVectorKnots[i]=mStartPoints.at(i);
        mAntiClockVectorKnots[i]=mStartPoints.at(i);
        mClockVectorKnots[i]=mStartPoints.at(i);
    }
    for(i=mNum*(mDensity-1); i<mNum*mDensity;i++ )
    {
        mVectorKnots[i]=mEndPoints.at(i-mNum*(mDensity-1));
        mAntiClockVectorKnots[i]=mEndPoints.at(i-mNum*(mDensity-1));
        mClockVectorKnots[i]=mEndPoints.at(i-mNum*(mDensity-1));
    }

    for(i=0;i<mNum-1;i++)
    {
        pointComplexors[i].x = mStartPoints[i+1].x()- mStartPoints[i].x();
        pointComplexors[i].y = mStartPoints[i+1].y()- mStartPoints[i].y();
        pointComplexors[i].setPolarByXY();
    }
    for(i=(mNum-1)*(mDensity-1);i< (mNum-1)*mDensity;i++)
    {
        pointComplexors[i].x = mEndPoints[i+1 -(mNum-1)*(mDensity-1) ].x()- mEndPoints[i-(mNum-1)*(mDensity-1) ].x();
        pointComplexors[i].y = mEndPoints[i+1-(mNum-1)*(mDensity-1) ].y()- mEndPoints[i-(mNum-1)*(mDensity-1) ].y();
        pointComplexors[i].setPolarByXY();
    }
    /*nornal case*/
    for(j=1;j<mDensity;j++)
    {
        float t= (float) j/(mDensity-1);
        for(i=0;i<mNum-1;i++)
        {
            pointComplexors[j*(mNum-1)+i].r=(1-t)*pointComplexors[i].r +
                                            t*pointComplexors[i+(mNum-1)*(mDensity-1)].r;
            pointComplexors[j*(mNum-1)+i].angle=(1-t)*pointComplexors[i].angle +
                                                t*pointComplexors[i+(mNum-1)*(mDensity-1)].angle;
            pointComplexors[j*(mNum-1)+i].startPoint=mVectorKnots[j*(mNum)+i];
            pointComplexors[j*(mNum-1)+i].setEndPoint();
            mVectorKnots[j*(mNum)+i+1]=pointComplexors[j*(mNum-1)+i].endPoint;
        }
    }
  //  qDebug("normal case done");
    /*anticlockwise case*/
    for(j=1;j<mDensity;j++)
    {
        float t= (float) j/(mDensity-1);
        for(i=0;i<mNum-1;i++)
        {
            if(pointComplexors[i+(mNum-1)*(mDensity-1)].angle <  pointComplexors[i].angle )
            {
                pointComplexors[j*(mNum-1)+i].angle=(1-t)*pointComplexors[i].angle +
                                                    t*(pointComplexors[i+(mNum-1)*(mDensity-1)].angle+2*M_PI);
            }
            pointComplexors[j*(mNum-1)+i].r=(1-t)*pointComplexors[i].r +
                                            t*pointComplexors[i+(mNum-1)*(mDensity-1)].r;
            pointComplexors[j*(mNum-1)+i].startPoint=mAntiClockVectorKnots[j*(mNum)+i];
            pointComplexors[j*(mNum-1)+i].setEndPoint();
            mAntiClockVectorKnots[j*(mNum)+i+1]=pointComplexors[j*(mNum-1)+i].endPoint;
        }
    }
   // qDebug("anticlockwise case done");
    /*clockwise case*/
    for(j=1;j<mDensity;j++)
    {
        float t= (float) j/(mDensity-1);
        for(i=0;i<mNum-1;i++)
        {
            if(pointComplexors[i+(mNum-1)*(mDensity-1)].angle > pointComplexors[i].angle )
            {
                pointComplexors[j*(mNum-1)+i].angle=(1-t)*(pointComplexors[i].angle+ 2*M_PI) +
                                                        t*pointComplexors[i+(mNum-1)*(mDensity-1)].angle;
            }
            pointComplexors[j*(mNum-1)+i].r=(1-t)*pointComplexors[i].r +
                                                t*pointComplexors[i+(mNum-1)*(mDensity-1)].r;
            pointComplexors[j*(mNum-1)+i].startPoint=mClockVectorKnots[j*(mNum)+i];
            pointComplexors[j*(mNum-1)+i].setEndPoint();
            mClockVectorKnots[j*(mNum)+i+1]=pointComplexors[j*(mNum-1)+i].endPoint;
        }
    }
   // qDebug("clockwise case done");

}
QPointF* Interpolation::getVectorLinear()
{
    return mVectorKnots;
}
QPointF* Interpolation::getClockVectorLinear()
{
    return mClockVectorKnots;
}

QPointF* Interpolation::getAntiClockVectorLinear()
{
    return mAntiClockVectorKnots;
}
Interpolation::~Interpolation()
{
    free(mLinearKnots);
    free(mVectorKnots);
    free(mAntiClockVectorKnots);
    free(mClockVectorKnots);
    //qDebug("delete Knots");
    QVector<QPointF> pNullPointVector1;
    mStartPoints.swap(pNullPointVector1);
    QVector<QPointF> pNullPointVector2;
    mEndPoints.swap(pNullPointVector2);
    //qDebug("delete Points");
    free(pointComplexors);
    //qDebug("delete pointComplexors");
}
