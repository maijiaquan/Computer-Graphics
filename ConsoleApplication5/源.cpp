#include "openglwindow.h"

/*
20170812 23.48
新算法已实现
*/

int g_ratio = 5;
int g_translation = 17;

int g_mouse_start_press_x = 0;
int g_mouse_start_press_y = 0;

int g_mouse_curr_x = 0;
int g_mouse_curr_y = 0;

int g_mouse_last_x = 0;
int g_mouse_last_y = 0;

bool g_is_mouse_press = false;

Camera &g_camera = Camera::instance();
Model g_model(Vector3(0,0,g_camera.dis_camera_model_)); 


struct ModelTransAttribute{
	float move_z;
	float rotate_theta;
	float scale;
}g_model_altered;

struct CameraTransAttribute{
	float move_x,move_y,move_z;
	float rotateX;	//相机绕X轴旋转的 角度数
	float rotateY;	//相机绕Y轴旋转的 角度数
	float rotateZ;
}g_camera_altered;



//构造函数初始化
openglwindow::openglwindow(QWidget *parent)
	: QOpenGLWidget(parent)
{
	this->setMouseTracking(true); 


	//初始化操作
	model_move_z = 0.0f;
	model_rotate_theta = 0.0f;
	model_scale = 0.0f;

	camera_move_z = 0.0f;
	camera_rotate_theta = 0.0f;
	camera_scale = 0.0f;
	camera_move_x = 0.0f;
	camera_move_y = 0.0f;
	camera_rotateX = 0.0f;
	camera_rotateY = 0.0f;
	camera_rotateZ = 0.0f;

	drag_theta_x_ = 0.0f;
	drag_theta_y_ = 0.0f;

	is_view_mode_ = false;

	startTimer(1000);    //设置间隔时间为1000ms，该函数返回值为1
	//startTimer(3000);  //如果有多个定时器，第几个定时器的返回值就是几
}

openglwindow::~openglwindow(){ }

//定时器函数
void openglwindow::timerEvent(QTimerEvent *t){//定时器时间
	g_camera.printCamereInfo();
	g_model.printModelInfo();

	cout<<"cal dis = "<<calculateDistance(g_model.world_position_.x_,
		g_model.world_position_.y_,
		g_model.world_position_.z_,
		g_camera.world_position_.x_,
		g_camera.world_position_.y_,
		g_camera.world_position_.z_)<<endl;
}

//鼠标事件：移动
void openglwindow::mouseMoveEvent(QMouseEvent *e){
	g_mouse_curr_x = e->x();
	g_mouse_curr_y = e->y();

	int mouse_change_x = g_mouse_curr_x - g_mouse_last_x;
	int mouse_change_y = g_mouse_curr_y - g_mouse_last_y;
	 

	g_mouse_last_x = g_mouse_curr_x;
	g_mouse_last_y = g_mouse_curr_y;
	if(g_is_mouse_press){
		update();
	}
	cout<<"mouse curr x = "<<g_mouse_curr_x<<"		mouse curr y = "<<g_mouse_curr_y<<endl;
	cout<<"mouse change x = "<<mouse_change_x<<"mouse change y = "<<mouse_change_y<<endl;

	drag_theta_x_ =  -mouse_change_x;
	drag_theta_y_ = mouse_change_y;

}

//鼠标事件：按住
void openglwindow::mousePressEvent(QMouseEvent *event){
	cout<<"press!!"<<endl;
	if(g_is_mouse_press == false){
		g_mouse_start_press_x = event->x();
		g_mouse_start_press_y = event->y();
		g_is_mouse_press = true;
	}
	update();
}

//鼠标事件：释放
void openglwindow::mouseReleaseEvent(QMouseEvent *event){
	cout<<"Release!!"<<endl;
	g_is_mouse_press = false;
}

//所有变换属性初始化
void openglwindow::transform_attribute_init(){
	g_model_altered.move_z = 0.0f;
	g_model_altered.rotate_theta = 0.0f;
	g_model_altered.scale = 1.0f;

	g_camera_altered.move_x = g_camera_altered.move_y = g_camera_altered.move_z = 0.0f;
	g_camera_altered.rotateX = g_camera_altered.rotateY = g_camera_altered.rotateZ = 0.0f;

	//drag_theta_x_ = 0.0f;
	//drag_theta_y_ = 0.0f;

	//垃圾代码，待修改
	model_move_z = 0.0f;
	model_rotate_theta = 0.0f;
	model_scale = 1.0f;

	camera_move_z = 0.0f; 
	camera_move_x = 0.0f;
	camera_move_y = 0.0f;
	camera_rotateX = 0.0f;
	camera_rotateY = 0.0f;
	camera_rotateZ = 0.0f;

	camera_rotate_theta = 0.0f;
	camera_scale = 0.0f;

	drag_theta_x_ = 0.0f;
	drag_theta_y_ = 0.0f;


}



//【关联】来自mainwindow的键盘事件
void openglwindow::changeAttribute(){

	//cout<<endl<<"g_model_altered.rotate_theta "<<g_model_altered.rotate_theta <<endl;

	g_model_altered.move_z = model_move_z;
	g_model_altered.rotate_theta = model_rotate_theta;
	g_model_altered.scale = model_scale;


	//cout<<g_camera_altered.move_x<<endl;
	//cout<<g_camera_altered.move_y<<endl;
	//cout<<g_camera_altered.move_z<<endl;
	//cout<<g_camera_altered.rotateX<<endl;
	//cout<<g_camera_altered.rotateY<<endl;


	g_camera_altered.move_x = camera_move_x;
	g_camera_altered.move_y = camera_move_y;
	g_camera_altered.move_z = camera_move_z;
	g_camera_altered.rotateX = camera_rotateX;	//绕X轴旋转的 角度数
	g_camera_altered.rotateY = camera_rotateY;	//绕Y轴旋转的 角度数       
	g_camera_altered.rotateZ = camera_rotateZ;


	//cout<<endl<<g_camera_altered.move_x<<endl;
	//cout<<g_camera_altered.move_y<<endl;
	//cout<<g_camera_altered.move_z<<endl;
	//cout<<g_camera_altered.rotateX<<endl;
	//cout<<g_camera_altered.rotateY<<endl;


	g_camera.total_rotate_theta_x_ += camera_rotateX;
	g_camera.total_rotate_theta_y_ += camera_rotateY;

}

//画线函数
void openglwindow::drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	glBegin(GL_LINES); ///在下面2个点之间画一条虚线  
	glVertex2f(x1 - windowWidth_/2, -y1 + windowHeight_/2);  
	glVertex2f(x2 - windowWidth_/2, -y2 + windowHeight_/2); 
	glEnd(); //结束 
}


void openglwindow::initializeGL(){  
	setWindowSize(600,600);
	setGeometry(10, 10, windowWidth_, windowHeight_); 	//设置widget的坐标和尺寸  

	glClearColor(0.0, 0.0, 0.0, 0);//设置清除时颜色  

	glClearDepth(1.0);//设置深度缓存  
	glEnable(GL_DEPTH_TEST);//允许深度测试  
	glDepthFunc(GL_LEQUAL);//设置深度测试类型  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正 
	rectX_ = 1.0;

	initCube();
	transform_attribute_init();
} 

void openglwindow::resizeGL(int w, int h){  
	glViewport(0,0,(GLsizei)w,(GLsizei)h);    	//视口变换  
	glMatrixMode(GL_PROJECTION);    	//投影变换  
	glLoadIdentity();  
	gluPerspective(90.0,(GLdouble)w/(GLdouble)h,0.1,10000.0);  
	//cout<<"glx = "<<(GLdouble)w<<" gly = "<<(GLdouble)h<<endl;

	//视图变换  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();  
	gluLookAt(0.0,0.0,300,
		0.0,0.0,0.0,
		0.0,1.0,0.0);  
}  

//////////////////////////////////////////////////////
void openglwindow::paintGL(){  
	glClear(GL_COLOR_BUFFER_BIT);  	//清屏  
	glColor3f(255.0,255.0,255.0);  

	drawCube();
	transform_attribute_init();   //要紧跟着drawCube()

	drawLine(g_mouse_start_press_x, g_mouse_start_press_y, g_mouse_curr_x, g_mouse_curr_y);
	glBegin(GL_POINTS); //绘制点  
	glVertex2f(0,0);  
	glVertex2f(0.6,0.6); 
	glEnd();  
	glFlush();  

	//glutSwapBuffers();
}  

//初始化立方体
void openglwindow::initCube(){
	int cube_vertex[24][3] = { 
		{-1,1,-1},{-1,-1,-1},{1,-1,-1},{1,1,-1},
		{1,1,-1},{1,-1,-1},{1,-1,1},{1,1,1},
		{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1},
		{-1,1,1},{-1,-1,1},{-1,-1,-1},{-1,1,-1},
		{-1,1,1},{-1,1,-1},{1,1,-1},{1,1,1},
		{-1,-1,-1},{-1,-1,1},{1,-1,1},{1,-1,-1}
	};


	for (int i=0; i<24; ++i){   //遍历所有顶点
		Vector3 v(cube_vertex[i][0],cube_vertex[i][1],cube_vertex[i][2]);
		g_model.local_vertexes_.push_back(v);            //将所有顶点 保存至model的 局部坐标
		g_model.trans_vertexes_.push_back(v+g_model.world_position_); //将所有顶点 保存至model的 透视坐标，----------trans_vertexes_ = 局部坐标 + 世界坐标
		g_model.save_vertexes_.push_back(v+g_model.world_position_); //新增保存变量
	}

	// 模型空间旋转
	//note:定义旋转矩阵，分三个坐标轴
	Matrix model_rotateY_matrix;
	model_rotateY_matrix.setRotate(Vector3(0,1,0),45);
	Matrix model_rotateX_matrix;
	model_rotateX_matrix.setRotate(Vector3(1,0,0),45);
	Matrix model_rotateZ_matrix;
	model_rotateZ_matrix.setRotate(Vector3(0,0,1),45);

	Matrix matrix = model_rotateX_matrix * model_rotateY_matrix * model_rotateZ_matrix;

	int index2 = 0;
	for (auto &v : g_model.local_vertexes_){  //遍历model的 所有局部坐标 顶点
		v = v * matrix;     //【旋转】
		g_model.trans_vertexes_[index2] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = 局部坐标 + 世界坐标
		//g_model.trans_vertexes_[index2++].normal_ = v.normal_;   //法线有什么用？？？


		//cout<<"v1 x = "<<g_model.trans_vertexes_[index2].x_<<" v1 y = "<<g_model.trans_vertexes_[index2].y_<<endl;
		//这一步透视坐标正常
	}

	g_model.poly_indices_.push_back(TrangleIndex(0,1,2));
	g_model.poly_indices_.push_back(TrangleIndex(2,3,0));  //front
	g_model.poly_indices_.push_back(TrangleIndex(4,5,6));
	g_model.poly_indices_.push_back(TrangleIndex(6,7,4));  //right
	g_model.poly_indices_.push_back(TrangleIndex(8,9,10));
	g_model.poly_indices_.push_back(TrangleIndex(10,11,8));  //back
	g_model.poly_indices_.push_back(TrangleIndex(12,13,14));
	g_model.poly_indices_.push_back(TrangleIndex(14,15,12));  //left
	g_model.poly_indices_.push_back(TrangleIndex(16,17,18));
	g_model.poly_indices_.push_back(TrangleIndex(18,19,16));  //top
	g_model.poly_indices_.push_back(TrangleIndex(20,21,22));
	g_model.poly_indices_.push_back(TrangleIndex(22,23,20));  //button
}


void openglwindow::drawCube(){
	//4.1.1 模型空间平移
	Matrix model_move_matrix;    //定义平移矩阵
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0,0,g_model_altered.move_z));    //-----> 联系键盘事件
	g_model.world_position_ = g_model.world_position_ * model_move_matrix;     //【平移】

	//4.1.2 模型空间【旋转】
	Matrix model_rotate_matrix;    //定义旋转矩阵
	model_rotate_matrix.setRotate(Vector3(0,1,0),g_model_altered.rotate_theta);   //-----> 联系键盘事件

	//4.1.3 模型空间【缩放】
	Matrix model_scale_matrix;        //定义缩放矩阵
	model_scale_matrix.setScale(Vector3(g_model_altered.scale,g_model_altered.scale,g_model_altered.scale));   //-----> 联系键盘事件

	Matrix model_transform = model_rotate_matrix * model_scale_matrix;  //定义旋转矩阵 + 缩放矩阵

	if(is_view_mode_ == false){
		//4.1.4 转换到世界坐标系
		int index = 0;
		for (auto &v : g_model.local_vertexes_){    //遍历model的 所有局部坐标 顶点
			v = v * model_transform;       //模型【旋转】 + 【缩放】 所有局部坐标 顶点
			//不【旋转】、不【缩放】，这一句就是 v = v
			g_model.trans_vertexes_[index] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = 局部坐标 + 世界坐标
			//原来这里他妈的给我初始化了 trans_vertexes_ 2017.8.12 21.55
			++index;
		}
	}else{
		g_model.trans_vertexes_ = g_model.save_vertexes_;   //恢复存档 —— 使用 保存变量
	}

	//法二：
	//重置世界坐标
	g_camera.world_position_.x_ = 0;	
	g_camera.world_position_.y_ = 0;
	g_camera.world_position_.z_ = 0;
	//重置观察角度

	//g_camera.look_at_theta_ = Vector3(0,0,0);

	//g_camera.view_transform(g_model.trans_vertexes_);  //重新改造 model的 trans_vertexes_ 

	//g_camera.set_lookAt(Vector3(drag_theta_x_, drag_theta_y_, 0)); //改变camera 的 look_at_theta_
	
	g_camera.look_at_theta_ = Vector3(drag_theta_x_, drag_theta_y_,0);  //修改 相机 角度


	//	//4.2.1 相机【移动】  -----> 联系键盘事件
	//Matrix camera_move_matrix;   //定义 平移矩阵
	//camera_move_matrix.identify();
	//camera_move_matrix.setTranslation(Vector3(g_camera_altered.move_x,g_camera_altered.move_y,g_camera_altered.move_z));
	//g_camera.set_position(camera_move_matrix);  //乘以 平移矩阵

	//【平移】
	float dx, dy, dz;
	//【平移】——【绕y轴】
	if(can_x_){
		Matrix camera_translation_matrix_x;
		camera_translation_matrix_x.identify();
		dx = g_camera.dis_camera_model_ * Sin(drag_theta_x_);
		dz = g_camera.dis_camera_model_ * (1 - Cos(drag_theta_x_));
		camera_translation_matrix_x.setTranslation(Vector3(dx, 0, dz));
		g_camera.set_position(camera_translation_matrix_x);   //【平移】相机的世界坐标
		 
		g_camera.view_transform_translation(g_model.trans_vertexes_);  // 将相机的世界坐标 “反向” 平移到 trans_vertexes_
		g_camera.view_transform_rotate(g_model.trans_vertexes_, 0, 1, 0, g_camera.look_at_theta_.x_);  //先绕y轴旋转
	}

	//【平移】 —— 【绕x轴】
	//第二次【平移】
	//Vector3 normal_vector;
	//if(drag_theta_y_){
	//	Matrix camera_translation_matrix_y;
	//	camera_translation_matrix_y.identify();
	//	dy = g_camera.dis_camera_model_ * Sin(drag_theta_y_);
	//	float length = g_camera.dis_camera_model_ * Cos(drag_theta_y_); 
	//	cout<<"length =	"<<length<<endl;
	//	float gamma = g_camera.dis_camera_model_ - length;
	//	cout<<"gamma = "<<gamma<<endl;
	//	dx = - gamma * Sin(drag_theta_x_);
	//	cout<<"dx = "<<dx<<endl;
	//	dz = gamma * Cos(drag_theta_x_);
	//	cout<<"dy = "<<dy<<endl;
	//	camera_translation_matrix_y.setTranslation(Vector3(dx,dy,dz));
	//	g_camera.set_position(camera_translation_matrix_y);
	//}
	g_camera.world_position_.x_ = 0;	
	g_camera.world_position_.y_ = 0;
	g_camera.world_position_.z_ = 0;

	//重置观察角度
	//g_camera.look_at_theta_ = Vector3(0,0,0);
	//g_camera.look_at_theta_ = Vector3(drag_theta_x_, drag_theta_y_,0);


	if(can_y_){
		Matrix camera_translation_matrix_y;
		camera_translation_matrix_y.identify();
		dy = g_camera.dis_camera_model_ * Sin(drag_theta_y_);
		dz = g_camera.dis_camera_model_ * (1 - Cos(drag_theta_y_));
		camera_translation_matrix_y.setTranslation(Vector3(0, dy, dz));
		g_camera.set_position(camera_translation_matrix_y);   //【平移】

		g_camera.view_transform_translation(g_model.trans_vertexes_);  //【平移】一次
		g_camera.view_transform_rotate(g_model.trans_vertexes_, 1, 0, 0, -g_camera.look_at_theta_.y_);  //绕x轴旋转
	}


	g_model.save_vertexes_ = g_model.trans_vertexes_;   //保存变量：保存该变换操作后的 trans_vertexes


	//4.2.3 转换到相机坐标
	//if(drag_theta_y_){
	//	g_camera.view_transform_rotate(g_model.trans_vertexes_, 
	//		//-normal_vector.x_, 
	//		//-normal_vector.y_, 
	//		//-normal_vector.z_,
	//		1,0,0,
	//		-drag_theta_y_); 
	//}

	// 4.3.3 透视除法
	for (int i=0;i<g_model.trans_vertexes_.size();++i){
		//cout<<i<<" w_ =  "<<g_model.trans_vertexes_[i].w_<<endl;
		g_model.trans_vertexes_[i].x_ = g_model.trans_vertexes_[i].x_/g_model.trans_vertexes_[i].z_;  //分母：实现 近大远小
		g_model.trans_vertexes_[i].y_ = g_model.trans_vertexes_[i].y_/g_model.trans_vertexes_[i].z_; 
		//model.trans_vertexes_[i].position_.z_ = model.trans_vertexes_[i].position_.z_/model.trans_vertexes_[i].position_.w_; 
		//model.trans_vertexes_[i].position_.w_ = 1/model.trans_vertexes_[i].position_.w_;
	}

	/************************************************************************/
	/* 4.4  窗口变换                                                         */
	/************************************************************************/
	int half_width = windowWidth_/2;
	int half_height = windowHeight_/2;
	for (int i=0;i<g_model.trans_vertexes_.size();++i){
		//cout<<"v1 x = "<<g_model.trans_vertexes_[i].x_<<" v1 y = "<<g_model.trans_vertexes_[i].y_<<endl;

		g_model.trans_vertexes_[i].x_ *= half_width;    //按比例放大
		g_model.trans_vertexes_[i].y_ *= half_height;   //按比例放大
		g_model.trans_vertexes_[i].x_ += half_width;    //使得模型位于屏幕正中间
		g_model.trans_vertexes_[i].y_ = half_height - g_model.trans_vertexes_[i].y_; //使得模型位于屏幕正中间
	}


	//绘制线框模型
	drawWireframeModel(g_model);
	if(is_view_mode_ == false){
		is_view_mode_ = true;
	}
}

void openglwindow::drawWireframeModel(Model & model){
	for (int index=0; index<model.poly_indices_.size(); ++index){  //遍历所有三角形
		//note:保存三角形的三个点————【从trans_vertexes_中取出点】

		Vector3 v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vector3 v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vector3 v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];

		//画出三角形的三条边
		drawLine(v1.x_,v1.y_,v2.x_,v2.y_);

		drawLine(v3.x_,v3.y_,v2.x_,v2.y_);
		drawLine(v1.x_,v1.y_,v3.x_,v3.y_);
		//cout<<"v1 x = "<<v1.x_<<" v1 y = "<< v1.y_<<endl;
		//cout<<"v2 x = "<<v2.x_<<" v2 y = "<< v2.y_<<endl;
		//cout<<"v3 x = "<<v3.x_<<" v3 y = "<< v3.y_<<endl;
	}
}
