#include "ArcBall.h"
#include <iostream>
#include <numbers>
#include <cmath>

ArcBall::ArcBall(Vector3d _eye , Vector3d _viewCenter, Vector3d _upDir, double _r ){
    eye = _eye; viewCenter = _viewCenter; upDir = _upDir;
    initialize();
}

void ArcBall::updateRidius(double _r){
    double r_new = (_r > 0)? r_step : -r_step;
    radius = std::max(r_min, std::min(r_max, radius + r_new));
}

void ArcBall::initialize(){
    radius = 1;
    mouseState = 2;
    angle = 0.0;
    camAxis = Vector3d(0,1,0);
    camPos = Eigen::Matrix4d::Identity();
}

void ArcBall::reset(){
    initialize();
    
}

void ArcBall::cb_mouse(int button, int state, double x, double y){
    if(button == 0){
        if(mouseState == 2 && state == 1){
            mouseClicked(x, y);
            return;
        }
        if(mouseState != 2 && state == 1){
            mouseMoved(x, y);
            return;
        }
        if(state == 0){
            mouseReleased();         
            return;
        }
    }else if(button == 1){
        
    }
}

//MagnumのQuaternionを使った行列の実装
//https://doc.magnum.graphics/magnum/classMagnum_1_1Math_1_1Quaternion.html#aae9f73fca12e7b81ff5b45bdb34cc039
void ArcBall::mouseMoved(double _x, double _y){
    Eigen::Vector3d curPoint = mappingOnSphere(_x, _y);
    angle = ((curPoint - befPoint).norm() < 1e-3)? 0.0: std::acos(curPoint.normalized().dot(befPoint.normalized()));  
    camAxis = befPoint.cross(curPoint).normalized(); 
}


void ArcBall::mouseClicked(double x, double y){
    mouseState = 0;
    angle = 0.0;
    befPoint = mappingOnSphere(x,y);
}

void ArcBall::mouseReleased(){
    mouseState = 2;
    angle = 0.0;
}

Eigen::Vector3d ArcBall::mappingOnSphere(double x, double y){
    if(x*x + y*y <= 1.0){
        double z = std::sqrt(1.0 - x*x - y*y);     
        return Eigen::Vector3d(x,y,z);
    }else{
        return Eigen::Vector3d(x,y,0).normalized();
    }
}

Eigen::Matrix4d ArcBall::getCameraPosition(){
    Eigen::Matrix4d R = Eigen::Matrix4d::Identity();
    Eigen::Matrix4d S = Eigen::Matrix4d::Identity();
    S(0,0) = S(1,1) = S(2,2) = radius;
    const double rad_speed = 0.03;
    R.block<3, 3>(0, 0) = Eigen::AngleAxisd(rad_speed * angle, camAxis).toRotationMatrix();  
    camPos *= R * S; 
    return camPos;
}