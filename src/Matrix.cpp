#include "Matrix.h"

using namespace Eigen;
std::vector<GLfloat> Mat2Array(const Eigen::MatrixXd& M){
    std::vector<GLfloat> v_array(M.rows() * M.cols());
    for(int i = 0; i < M.cols(); i++){
            for(int j = 0; j < M.rows(); j++)v_array[M.rows() * i + j] = M(j,i);// 行列を一列のベクトルに変換
        }
        return v_array;
}

Matrix4d getLookAt(const Vector3d&eye, const Vector3d& center, const Vector3d& up){
    Vector3d f = (center - eye).normalized();
    Vector3d s = f.cross(up).normalized();
    Vector3d u = s.cross(f);
    Matrix4d M;
    M << s.x(), s.y(), s.z(), -s.dot(eye),
    u.x(), u.y(), u.z(), -u.dot(eye),
    -f.x(), -f.y(), -f.z(), f.dot(eye),
    0, 0, 0, 1;
    return M;
}

Matrix4d getOrthogonal(const double left, const double right, const double bottom, 
                        const double top, const double near, const double far){
    Matrix4d M;
    M << 2.0 / (right - left), 0, 0, 0,
    0, 2.0 / (top - bottom), 0, 0,
    0, 0, -2.0 / (far - near), 0,
    0, 0, 0, 1;
    return M;
}

Matrix4d getPerspective(const double fovy, const double aspect, const double near, const double far){
    const double t = tan(fovy / 2.0);
    Matrix4d M;
    M << 1/ (aspect * t), 0, 0, 0,
    0, 1 / t, 0, 0,
    0, 0, -(far + near) / (far - near), -2.0 * far * near / (far - near),
    0, 0, -1, 0;
    return M;
}

Matrix4d getRotate(const double a, Eigen::Vector3d axis){
    axis = axis.normalized();
    Quaterniond quat;
    quat = AngleAxisd(a,axis);  //Y軸周りに90度反時計回りに回転
    Eigen::Matrix4d t = Eigen::Matrix4d::Identity();  // 4x4の単位行列を作成
    t.block<3,3>(0,0) = quat.matrix();;  // 左上の3x3ブロックにmを設定
    return t;
}