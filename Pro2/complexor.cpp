#include "complexor.h"

void Complexor::setPolarByXY(){
    r=sqrt(x*x+y*y);
    if(x>0&&y>=0) angle=atan(y/x);
    else if(x>0&&y<0) angle=atan(y/x);
    else if(x<0&&y>=0) angle=atan(y/x)+M_PI;
    else if(x<0&&y<0) angle=atan(y/x)-M_PI;
    else if(x==0&&y>0) angle= M_PI;
    else if (x==0&& y<0) angle= -M_PI;
}
void Complexor::setEndPoint(){
    endPoint.setX(startPoint.x()+r* cos(angle));
    endPoint.setY(startPoint.y()+r* sin(angle));
}
Complexor::~Complexor()
{

}
