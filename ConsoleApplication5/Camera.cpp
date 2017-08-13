#include "Camera.h"

void Camera::set_position(const Matrix &m){
	world_position_ = world_position_ * m;
}

//在原有基础上，再旋转一个角度
void Camera::set_lookAt(const Vector3& rotate_angle){
	look_at_theta_ = look_at_theta_+rotate_angle;
}

void Camera::view_transform(vector<Vertex> &vertexes){
	//Matrix view_move_matrix;
	//view_move_matrix.identify();
	//view_move_matrix.setTranslation(-1*world_position_);   //camera世界坐标的逆矩阵

	//Matrix view_rotateX_matrix;
	//view_rotateX_matrix.setRotate(1,look_at_theta_.y_);   //绕X轴旋转

	//Matrix view_rotateY_matrix;
	//view_rotateY_matrix.setRotate(2,look_at_theta_.x_);   //绕Y轴旋转


	//Matrix view_transform_matrix = view_move_matrix * 
	//	view_rotateX_matrix *
	//	view_rotateY_matrix;

	////cout<<"1----";
	//
	//for (auto &v : vertexes){
	//	//cout<<"1----";
	//	//v.printVertex();

	//	v.position_ *= view_transform_matrix;
	//	
	//	//cout<<"2----";
	//	//v.printVertex();

	//	//w == 1

	//}
}

void Camera::view_transform(vector<Vector3> &vertexes){      //新增重载版本：Vector3  2017.8.8
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera世界坐标的逆矩阵，
	//相机远离原点，trans_vertexes跟着远离，
	//相机靠近原点，trans_vertexes跟着靠近

	Matrix view_rotateX_matrix;
	//view_rotateX_matrix.setRotate(1,look_at_theta_.y_);   //绕X轴旋转
	//view_rotateX_matrix.setRotate(Vector3(1,0,0),look_at_theta_.y_);   //绕X轴旋转
	view_rotateX_matrix.setRotateAny(Vector3(1,0,0),look_at_theta_.y_);   //绕X轴旋转

	Matrix view_rotateY_matrix;
	//view_rotateY_matrix.setRotate(2,look_at_theta_.x_);   //绕Y轴旋转
	//view_rotateY_matrix.setRotate(Vector3(0,1,0),look_at_theta_.x_);   //绕Y轴旋转
	//view_rotateY_matrix.setRotateAny(Vector3(0,1,0),look_at_theta_.x_);   //绕Y轴旋转
	
	//view_rotateY_matrix.setRotateAny(Vector3(0,1,0),look_at_theta_.x_);   //绕Y轴旋转
//	view_rotateY_matrix.setRotateAny(Vector3(sqrt(2)/2,0,sqrt(2)/2),look_at_theta_.x_);   //绕Y轴旋转
	view_rotateY_matrix.setRotateAny(Vector3(0, 1, 0),look_at_theta_.x_);   //绕Y轴旋转

	Matrix view_transform_matrix = view_move_matrix * view_rotateX_matrix *	view_rotateY_matrix;

	for (auto &v : vertexes){
		//v.printVertex();
		v *= view_transform_matrix;
		//v.printVertex();
	}
}

//1.1.该版本只负责实现“反向“【平移】
void Camera::view_transform_translation(vector<Vector3> &vertexes){   //只负责【平移】       //新重载版本 引入坐标轴：Vector3  2017.8.11  18:25
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera世界坐标的逆矩阵，

	for (auto &v : vertexes){
		v *= view_move_matrix;
	}
}

//1.2.该版本只负责实现【旋转】
void Camera::view_transform_rotate(vector<Vector3> &vertexes, float x, float y, float z, float theta){      //新重载版本 引入坐标轴：Vector3  2017.8.11  18:25
	Matrix view_rotate_matrix;
	view_rotate_matrix.setRotateAny(Vector3(x,y,z),theta);   //绕任意轴旋转

	for (auto &v : vertexes){
		v *= view_rotate_matrix;
	}
}

//2.1. “反向“【平移】 重载版本，引入保存矩阵      
void Camera::view_transform_translation(vector<Vector3> &vertexes, Matrix &m){   
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera世界坐标的逆矩阵，

	for (auto &v : vertexes){
		v *= view_move_matrix;
	}
	m = m * view_move_matrix;
}

//2.2.【旋转】 重载版本，引入保存矩阵
void Camera::view_transform_rotate(vector<Vector3> &vertexes, Matrix &m, float x, float y, float z, float theta){      //新重载版本 引入坐标轴：Vector3  2017.8.11  18:25
	Matrix view_rotate_matrix;
	view_rotate_matrix.setRotateAny(Vector3(x,y,z),theta);   //绕任意轴旋转

	for (auto &v : vertexes){
		v *= view_rotate_matrix;
	}

	m = m * view_rotate_matrix;
}


//转换到cvv裁剪空间
void Camera::canonicalview_volume(vector<Vertex> &vertexes){
	Matrix cvv_matrix;
	cvv_matrix.identify();

	cvv_matrix.m11 = Tan(angle_/2)/w_h_;       //zoomx
	cvv_matrix.m22 = Tan(angle_/2);			   //zoomy

	//cvv_matrix.m33 = far_ / (far_ - near_);
	//cvv_matrix.tz = - far_*near_/(far_ - near_);
	//cvv_matrix.m33 = (far_+near_)/(far_-near_);
	//cvv_matrix.tz = 2*near_*far_/(near_-far_);
	cvv_matrix.m44 = 0;
	cvv_matrix.m34 = 1;
	cvv_matrix.printMatrix();
	for (auto &v : vertexes){
		v.position_ *= cvv_matrix;   //w发生变化，且w_ = z_
	}
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int Camera::transform_check_cvv(const Vector3 &v) {
	int check = 0;
	if (v.z_ < -1.0f) check |= 1;
	if (v.z_ >  1.0f) check |= 2;
	if (v.x_ < -1.0f) check |= 4;
	if (v.x_ >  1.0f) check |= 8;
	if (v.y_ < -1.0f) check |= 16;
	if (v.y_ >  1.0f) check |= 32;
	return check;
}

bool Camera::is_back(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3){
	Vector3 vec13 = v3-v1;
	Vector3 vec12 = v2-v1;
	Vector3 vv = cross_product(vec13,vec12);
	return (vv*v1)>0;
}