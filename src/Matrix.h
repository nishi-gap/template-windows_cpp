#pragma once
#include <Eigen/Dense>

#include <glad/gl.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace Eigen;
std::vector<GLfloat> Mat2Array(const Eigen::MatrixXd& M);
Matrix4d getLookAt(const Vector3d&eye, const Vector3d& center, const Vector3d& up);
Matrix4d getOrthogonal(const double left, const double right, const double bottom, 
                        const double top, const double near, const double far);
Matrix4d getPerspective(const double fovy, const double aspect, const double near, const double far);
Matrix4d getRotate(const double a, Eigen::Vector3d axis);