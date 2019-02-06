#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    flag=1;
    tempPointNum=0;
    mDrawTimes=0;
    canPlay=false;
    isGen=false;

    isPlay=false;
    Knots= NULL;
    mInterpolation=NULL;
    tick=0;
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timerInterpolation()));
    genHand();
    genPen();
    this->setWindowTitle("Interpolation Demo");
}
void MainWindow::paintEvent(QPaintEvent *ev)
{
    QFile qssfile(":/base.qss");//为UI控件设置样式
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);

    QPainter p(this);
    //在界面的左边部分设置背景为白色，表示可接受鼠标点击的绘画区域
    p.setClipRect(5,0,size().width()-140,size().height());
    p.fillRect(QRectF(0, 0, size().width(), size().height()), Qt::white);

    p.setPen(QPen(QColor(200, 0 ,0), 2));
    p.setRenderHint(QPainter::Antialiasing, true);// 反走样

    for (int i=0 ;i<mPoints.size();i++)
    {
        p.drawEllipse(mPoints.at(i),5,5);
    }
    p.setPen(QPen(QColor(100,100,100), 2));


   if(mDrawTimes!=0)
    {
       for(int i=0;i<mStartP.size()-1;i++)
       {
            p.drawLine(mStartP.at(i), mStartP.at(i+1));//绘制初始图形关键点之间的直线
       }
        if(mStartP.size()>2)  p.drawLine(mStartP.at(mStartP.size()-1), mStartP.at(0));
    }
    if(mDrawTimes==2)
    {
        for(int i=0;i<mEndP.size()-1;i++)
        {
             p.drawLine(mEndP.at(i), mEndP.at(i+1));//绘制终止图形关键点之间的直线
        }
        if(mEndP.size()>2) p.drawLine(mEndP.at(mEndP.size()-1), mEndP.at(0));
    }

    if(isPlay)
    {
        p.setPen(QPen(QColor(200, 0 ,0), 2));
        for (int i=0 ;i<mInterpolation->mNum;i++)
        {
            p.drawEllipse(Knots[i+mInterpolation->mNum*tick],5,5);
        }
        p.setPen(QPen(QColor(100,100,100), 2));
        for (int i=0 ;i<mInterpolation->mNum-1;i++)
        {
            p.drawLine(Knots[i+mInterpolation->mNum*tick], Knots[i+1+mInterpolation->mNum*tick]);//绘制插值点之间的直线
        }
        if(mInterpolation->mNum>2) p.drawLine(Knots[mInterpolation->mNum-1+ mInterpolation->mNum*tick],Knots[mInterpolation->mNum*tick]);
    }


}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(flag==1)//可接收点击画点的模式
    {   //如果鼠标的点击位置是绘画区域
        if(mDrawTimes>2)
        {
            return;
        }
        if((ev->pos().x() <size().width()-140)&& (ev->pos().x() >0 )
                && (ev->pos().y() < (size().height())) &&  (ev->pos().y() >0))
        {
            mPoints.append(ev->pos());
           // qDebug("%f,%f,",(double)ev->pos().x(),(double)ev->pos().y());
            tempPointNum++;
            if(mDrawTimes==0)
            {
                mStartP.append(ev->pos());
            }else if(mDrawTimes==1)
            {
                mEndP.append(ev->pos());
            }
            update();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_End_Draw_clicked()
{
    if(tempPointNum<=0)
    {
        return;
    }
    canPlay=false;
    mPointNum.append(tempPointNum);//此次绘制的点的个数
    mDrawTimes++;//绘制次数++
    tempPointNum=0;
    if(mDrawTimes==2)
    {
        qDebug("%d",mDrawTimes);
        if(mPointNum.at(0)== mPointNum.at(1))
        {
            canPlay=true;
        }else{
            qDebug("The points number doesn't match in two times.");
        }
    }else{
       qDebug("You doesn't finish drawing. You can't play.");
    }
}

void MainWindow::on_Play_clicked()
{
    if(canPlay){
        flag=0; //停止绘制，不能接收屏幕上鼠标点击
        if(isGen==false)//生成插值点
        {
            if(mInterpolation!=NULL) delete mInterpolation;
            mInterpolation=new Interpolation(mPoints,mPointNum.at(0),ui->Density->value());
            ui->Density->setReadOnly(true);
            isGen=true;
        }
        if(ui->Linear->isChecked()==true)//线性插值模式Play
        {
           Knots=mInterpolation->getLinear();
        }else if(ui->Vector->isChecked()==true)//矢量线性插值模式Play
        {
            Knots=mInterpolation->getVectorLinear();
        }
        else if(ui->Vector_Anticlock->isChecked()==true)//逆时针矢量线性插值模式Play
        {
            Knots=mInterpolation->getAntiClockVectorLinear();
        }else if(ui->Vector_Clock->isChecked()==true)//顺时针矢量线性插值模式Play
        {
            Knots=mInterpolation->getClockVectorLinear();
        }
        isPlay=true;
        tick=0;
        timer->stop();
        timer->start();
        timer->setInterval(ui->Interval->value());
    }
}

void MainWindow::on_Clear_clicked()
{
    if(isPlay) return;
    clearAll();
}
void MainWindow::clearAll(){
    flag=1;
    mDrawTimes=0;
    tempPointNum=0;
    canPlay=false;
    isGen=false;
    isPlay=false;

    tick=0;
    QVector<int> pNullIntVector;
    mPointNum.swap(pNullIntVector);
    //qDebug("mPointNumLength: %d ", mPointNum.length());
    QVector<QPointF> pNullPointVector;
    mPoints.swap(pNullPointVector);
   // qDebug("mPointsLength: %d ",mPoints.length() );
     QVector<QPointF> pNullPointVector1;
    mStartP.swap(pNullPointVector1);
    QVector<QPointF> pNullPointVector2;
    mEndP.swap(pNullPointVector2);
    if(mInterpolation!=NULL)
    {
        delete mInterpolation;
        mInterpolation=NULL;
       // qDebug("delete mInterpolation");
    }
    ui->Density->setReadOnly(false);
    Knots=NULL;
}
void MainWindow::timerInterpolation()
{
    tick+=1;
    if(tick > mInterpolation->mDensity-1)
    {
        timer->stop();
        isPlay=false;
        return;
    }
    update();
}
void MainWindow::genHand(){
    float handpoint[30][2]={
        528.000000,303.000000,
        319.000000,302.000000,
        267.000000,305.000000,
        200.000000,299.000000,
        240.000000,279.000000,
        168.000000,266.000000,
        232.000000,253.000000,
        156.000000,229.000000,
        233.000000,222.000000,
        153.000000,176.000000,
        233.000000,188.000000,
        218.000000,146.000000,
        269.000000,164.000000,
        310.000000,210.000000,
        517.000000,225.000000,
        436.000000,301.000000,
        438.000000,141.000000,
        418.000000,113.000000,
        395.000000,52.000000,
        430.000000,70.000000,
        432.000000,23.000000,
        454.000000,58.000000,
        474.000000,10.000000,
        494.000000,59.000000,
        522.000000,8.000000,
        525.000000,67.000000,
        575.000000,44.000000,
        564.000000,86.000000,
        513.000000,138.000000,
        521.000000,301.000000
    };

    for (int i=0;i<30;i++)
    {
        QPointF point;
        point.setX(handpoint[i][0]);point.setY(handpoint[i][1]+40);
        mHandPoints.append(point);
    }
}
void MainWindow::genPen(){
    float penpoint[14][2]={
        50.000000,308.000000,
        166.000000,305.000000,
        168.000000,341.000000,
        240.000000,268.000000,
        167.000000,206.000000,
        164.000000,245.000000,
        51.000000,247.000000,
        680.000000,230.000000,
        574.000000,231.000000,
        555.000000,188.000000,
        491.000000,261.000000,
        570.000000,333.000000,
        568.000000,295.000000,
        679.000000,290.000000,
    };

    for (int i=0;i<14;i++)
    {
        QPointF point;
        point.setX(penpoint[i][0]);point.setY(penpoint[i][1]+40);
        mPenPoints.append(point);
    }
}

void MainWindow::on_Hand_clicked()
{
    if(isPlay){return;}
    clearAll();
    flag=0;
    canPlay=true;
    for(int i=0;i<mHandPoints.size();i++)
    {
        mPoints.append(mHandPoints.at(i));
        if(i<mHandPoints.size()/2)
        {
            mStartP.append(mHandPoints.at(i));//初始图形的点
        }else{
            mEndP.append(mHandPoints.at(i));//终止图形的点
        }
    }
    mDrawTimes=2;
    mPointNum.append(mHandPoints.size()/2);//初始图形绘制的点的个数
    mPointNum.append(mHandPoints.size()/2);//终止图形绘制的点的个数
}


void MainWindow::on_Pen_clicked()
{
    if(isPlay){return;}
    clearAll();
    flag=0;
    canPlay=true;
    for(int i=0;i<mPenPoints.size();i++)
    {
        mPoints.append(mPenPoints.at(i));
        if(i<mPenPoints.size()/2)
        {
            mStartP.append(mPenPoints.at(i));//初始图形的点
        }else{
            mEndP.append(mPenPoints.at(i));//终止图形的点
        }
    }
    mDrawTimes=2;
    mPointNum.append(mPenPoints.size()/2);//初始图形绘制的点的个数
    mPointNum.append(mPenPoints.size()/2);//终止图形绘制的点的个数
}
