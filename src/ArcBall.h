#pragma once
#include <Eigen/Dense>
#include <cmath>
using namespace Eigen;
//https://github.com/mosra/magnum-examples/tree/master/src/arcball
class ArcBall {
public:
    ArcBall(Vector3d _eye = Vector3d(0,-1,0), Vector3d _viewCenter = Vector3d(0,0,0), 
    Vector3d _upDir = Vector3d(0,1,0), double _r = 10);
    
    Vector3d eye;// 視点ベクトル
    Vector3d viewCenter;//視点の中心
    Vector3d upDir;// 上方向ベクトル

    void reset();
    void updateRidius(double _r);    
    void cb_mouse(int button, int state, double x, double y);//button: 0: left, 1: right, 2: middle. state: 1: clicked, 0:released
    Eigen::Matrix4d getCameraPosition();
    
    
protected:
    int mouseState;//0: clicked, 1: moved, 2: released
    double radius;// 半径
    double r_step = 0.01;
    double r_max = 10, r_min = 0.1;
    Eigen::Vector3d befPoint;
    Eigen::Vector3d camAxis;
    Eigen::Matrix4d camPos;
    double angle;

    void initialize();
    void mouseMoved(double x, double y);
    void mouseClicked(double x, double y);
    void mouseReleased();
    Eigen::Vector3d mappingOnSphere(double x, double y);//(x,y)は正規化された [-1,1]の範囲にあること
    
};