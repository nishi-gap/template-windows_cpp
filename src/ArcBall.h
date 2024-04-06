#pragma once
#include <Eigen/Dense>
using namespace Eigen;
//https://github.com/mosra/magnum-examples/tree/master/src/arcball
class ArcBall {
public:
    double r;// 半径
    Vector3d eye;// 中心へのベクトル
    Vector3d vertical;// 三次元空間上の上方向
    Vector3d horizon;// 水平方向
    ArcBall(Vector3d _eye = Vector3d(0,0,-1), Vector3d _vertical = Vector3d(0,1,0), Vector3d _horizon = Vector3d(1,0,0), double _r = 10)
    :r(_r), eye(_eye), vertical(_vertical), horizon(_horizon){}
    void updateRidius(double _r);
    void mouseMoved(double x, double y);
private:
    void updateEye();
    
};