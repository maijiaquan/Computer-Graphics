#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include "Vector3.h"
#include "Matrix.h"
#include "Vertex.h"
using namespace std;

class Camera
{
public:
	static Camera& instance()
	{
		static Camera *camera = new Camera(); 
		return *camera;
	}
	void set_position(const Matrix &m);
	void set_lookAt(const Vector3& point);

	void canonicalview_volume(vector<Vertex> &vertexes);
	void view_transform(vector<Vertex> &vertexes);
	void view_transform(vector<Vector3> &vertexes);
	//void view_transform(vector<Vector3> &vertexes, float x, float y, float z, float theta);
	
	//1
	void view_transform_translation(vector<Vector3> &vertexes);   //只负责【平移】
	void view_transform_rotate(vector<Vector3> &vertexes, float x, float y, float z, float theta); //只负责【旋转】

	//2：重载1，把保存矩阵 传进去 再带出来
	void view_transform_translation(vector<Vector3> &vertexes, Matrix &m);   //只负责【平移】
	void view_transform_rotate(vector<Vector3> &vertexes, Matrix &m, float x, float y, float z, float theta); //只负责【旋转】

	int transform_check_cvv(const Vector3 &v);
	bool is_back(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3);

	void set_w_h(float w_h){
		w_h_ = w_h;
	}
	float get_far()const{
		return far_;
	}
	float get_near()const{
		return near_;
	}

	Vector3 get_position(){
		return world_position_;
	}

	Vector3 get_lookat(){
		return look_at_theta_;
	}

	void printCamereInfo(){
		cout<<endl<<"------------- Camere Info  --------------------"<<endl;
		cout<<"|	camera x = "<<world_position_.x_<<endl;
		cout<<"|	camera y = "<<world_position_.y_<<endl;
		cout<<"|	camera z = "<<world_position_.z_<<endl;

		cout<<"|	camera z - 10 = "<<world_position_.z_ - 10<<endl;
		cout<<"|	camera x / 10 = "<<world_position_.x_/10<<endl;
		cout<<"|	camera y / 10 = "<<world_position_.y_/10<<endl;
		cout<<"|	camera z / 10 = "<<(world_position_.z_ - 10)/10<<endl;
		cout<<"|	camera look_at_theta_.x_ = "<<look_at_theta_.x_<<endl;
		cout<<"|	camera look_at_theta_.y_ = "<<look_at_theta_.y_<<endl;
		cout<<"|	camera look_at_theta_.z_ = "<<look_at_theta_.z_<<endl;
		cout<<"-----------------------------------------------"<<endl;
	}

	float dis_camera_model_;
	Vector3 world_position_;     //世界坐标 一个点


	float total_rotate_theta_x_;  //计算旋转总和
	float total_rotate_theta_y_;

	float last_dx;
	float last_dz;
	Vector3 look_at_theta_;      //观察角度  一个点（向量）

private:
	Camera():
		world_position_(),
		look_at_theta_(Vector3(0,0,0)),
		angle_(90),
		near_(1),
		far_(10000),
		dis_camera_model_(10),
		total_rotate_theta_x_(0.0),
		total_rotate_theta_y_(0.0),
		last_dx(0.0),
		last_dz(0.0)
	{}

	float angle_;        //Y轴方向视角度数
	float w_h_;          //屏幕宽高比
	float near_;
	float far_;
};



#endif // !CAMERA_H_
