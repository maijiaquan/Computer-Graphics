#include "Camera.h"

void Camera::set_position(const Matrix &m){
	world_position_ = world_position_ * m;
}

//��ԭ�л����ϣ�����תһ���Ƕ�
void Camera::set_lookAt(const Vector3& rotate_angle){
	look_at_theta_ = look_at_theta_+rotate_angle;
}

void Camera::view_transform(vector<Vertex> &vertexes){
	//Matrix view_move_matrix;
	//view_move_matrix.identify();
	//view_move_matrix.setTranslation(-1*world_position_);   //camera��������������

	//Matrix view_rotateX_matrix;
	//view_rotateX_matrix.setRotate(1,look_at_theta_.y_);   //��X����ת

	//Matrix view_rotateY_matrix;
	//view_rotateY_matrix.setRotate(2,look_at_theta_.x_);   //��Y����ת


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

void Camera::view_transform(vector<Vector3> &vertexes){      //�������ذ汾��Vector3  2017.8.8
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera��������������
	//���Զ��ԭ�㣬trans_vertexes����Զ�룬
	//�������ԭ�㣬trans_vertexes���ſ���

	Matrix view_rotateX_matrix;
	//view_rotateX_matrix.setRotate(1,look_at_theta_.y_);   //��X����ת
	//view_rotateX_matrix.setRotate(Vector3(1,0,0),look_at_theta_.y_);   //��X����ת
	view_rotateX_matrix.setRotateAny(Vector3(1,0,0),look_at_theta_.y_);   //��X����ת

	Matrix view_rotateY_matrix;
	//view_rotateY_matrix.setRotate(2,look_at_theta_.x_);   //��Y����ת
	//view_rotateY_matrix.setRotate(Vector3(0,1,0),look_at_theta_.x_);   //��Y����ת
	//view_rotateY_matrix.setRotateAny(Vector3(0,1,0),look_at_theta_.x_);   //��Y����ת
	
	//view_rotateY_matrix.setRotateAny(Vector3(0,1,0),look_at_theta_.x_);   //��Y����ת
//	view_rotateY_matrix.setRotateAny(Vector3(sqrt(2)/2,0,sqrt(2)/2),look_at_theta_.x_);   //��Y����ת
	view_rotateY_matrix.setRotateAny(Vector3(0, 1, 0),look_at_theta_.x_);   //��Y����ת

	Matrix view_transform_matrix = view_move_matrix * view_rotateX_matrix *	view_rotateY_matrix;

	for (auto &v : vertexes){
		//v.printVertex();
		v *= view_transform_matrix;
		//v.printVertex();
	}
}

//1.1.�ð汾ֻ����ʵ�֡����򡰡�ƽ�ơ�
void Camera::view_transform_translation(vector<Vector3> &vertexes){   //ֻ����ƽ�ơ�       //�����ذ汾 ���������᣺Vector3  2017.8.11  18:25
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera��������������

	for (auto &v : vertexes){
		v *= view_move_matrix;
	}
}

//1.2.�ð汾ֻ����ʵ�֡���ת��
void Camera::view_transform_rotate(vector<Vector3> &vertexes, float x, float y, float z, float theta){      //�����ذ汾 ���������᣺Vector3  2017.8.11  18:25
	Matrix view_rotate_matrix;
	view_rotate_matrix.setRotateAny(Vector3(x,y,z),theta);   //����������ת

	for (auto &v : vertexes){
		v *= view_rotate_matrix;
	}
}

//2.1. �����򡰡�ƽ�ơ� ���ذ汾�����뱣�����      
void Camera::view_transform_translation(vector<Vector3> &vertexes, Matrix &m){   
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);   //camera��������������

	for (auto &v : vertexes){
		v *= view_move_matrix;
	}
	m = m * view_move_matrix;
}

//2.2.����ת�� ���ذ汾�����뱣�����
void Camera::view_transform_rotate(vector<Vector3> &vertexes, Matrix &m, float x, float y, float z, float theta){      //�����ذ汾 ���������᣺Vector3  2017.8.11  18:25
	Matrix view_rotate_matrix;
	view_rotate_matrix.setRotateAny(Vector3(x,y,z),theta);   //����������ת

	for (auto &v : vertexes){
		v *= view_rotate_matrix;
	}

	m = m * view_rotate_matrix;
}


//ת����cvv�ü��ռ�
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
		v.position_ *= cvv_matrix;   //w�����仯����w_ = z_
	}
}

// ����������ͬ cvv �ı߽�������׶�ü�
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