#include "openglwindow.h"

/*
20170814 20.09
����ѡ���ˣ�������bug��������ƶ�Ҳ���϶�������ת

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


int g_selected_vertex_index = 0;


struct ModelTransAttribute{
	float move_z;
	float rotate_theta;
	float scale;
}g_model_altered;

struct CameraTransAttribute{
	float move_x,move_y,move_z;
	float rotateX;	//�����X����ת�� �Ƕ���
	float rotateY;	//�����Y����ת�� �Ƕ���
	float rotateZ;
}g_camera_altered;

ObjData g_objdata;

//ObjData g_objdata("E:\\bunny.obj");

//���캯����ʼ��
openglwindow::openglwindow(QWidget *parent)
	: QOpenGLWidget(parent)
{
	this->setMouseTracking(true); 


	//��ʼ������
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
	drag_theta_z_ = 0.0f;

	is_view_mode_ = false;
	is_ctrl = false;

	startTimer(1000);    //���ü��ʱ��Ϊ1000ms���ú�������ֵΪ1
	//startTimer(3000);  //����ж����ʱ�����ڼ�����ʱ���ķ���ֵ���Ǽ�
	save_matrix.identify();    //��ʼ�� �������
}

openglwindow::~openglwindow(){ }

//��ʱ������
void openglwindow::timerEvent(QTimerEvent *t){//��ʱ��ʱ��
	//g_camera.printCamereInfo();
	//g_model.printModelInfo();

	//cout<<"cal dis = "<<calculateDistance(g_model.world_position_.x_,
	//	g_model.world_position_.y_,
	//	g_model.world_position_.z_,
	//	g_camera.world_position_.x_,
	//	g_camera.world_position_.y_,
	//	g_camera.world_position_.z_)<<endl;
	//g_objdata.printObj();

	//drag_theta_x_ = 0.1;
	//update();
}

//����¼����ƶ�
void openglwindow::mouseMoveEvent(QMouseEvent *e){
	cout<<"index.x = "<<g_model.trans_vertexes_[g_selected_vertex_index].x_ - windowWidth_/2<<" y = "<<windowHeight_/2-g_model.trans_vertexes_[g_selected_vertex_index].y_<<endl;
	//glVertex2f(x2 - windowWidth_/2, -y2 + windowHeight_/2); 
	//
	g_mouse_curr_x = e->x();
	g_mouse_curr_y = e->y();

	int mouse_change_x = g_mouse_curr_x - g_mouse_last_x;
	int mouse_change_y = g_mouse_curr_y - g_mouse_last_y;


	g_mouse_last_x = g_mouse_curr_x;
	g_mouse_last_y = g_mouse_curr_y;

	//cout<<"mouse curr x = "<<g_mouse_curr_x<<"		mouse curr y = "<<g_mouse_curr_y<<endl;
	//cout<<"mouse change x = "<<mouse_change_x<<"mouse change y = "<<mouse_change_y<<endl;

	drag_theta_x_ =  -mouse_change_x;
	drag_theta_y_ = mouse_change_y;
	if(g_is_mouse_press){
		update();
	}
	//update();
	//��ѡ�㡿���������е�
	g_selected_vertex_index = 0;
	for (int i=0; i<g_model.trans_vertexes_.size(); ++i){
		if(
			(
			(g_mouse_curr_x >= (g_model.trans_vertexes_[i].x_ - 1)) 
			&&
			(g_mouse_curr_x <= (g_model.trans_vertexes_[i].x_ + 1)) 
			)

			&& 	
			(
			(g_mouse_curr_y >= (g_model.trans_vertexes_[i].y_ -1))
			&&
			(g_mouse_curr_y <= (g_model.trans_vertexes_[i].y_ + 1))
			)
			){
				g_selected_vertex_index = i;

				////��Բ
				////glColor3f(255.0,0,0);  

				//glBegin(GL_POLYGON);  
				////	glBegin(GL_LINE_LOOP);
				//const GLfloat R = 3.0f;  
				//const int n = 10;  

				//for (int j = 0; j < n; j++)  
				//{  
				//	glVertex2f(R*cos(2*PI/n*j)  , R*sin(2*PI/n*j) );    
				//}  
				////		cout<<"oval x = "<< R*cos(2*PI/n*j)+g_mouse_curr_x  - windowWidth_/2<< " y = "<<R*sin(2*PI/n*)+g_mouse_curr_y - windowWidth_/2<<endl;

				//glEnd(); 

				///////// oval ///////////////
				glColor3f(255.0,0.0,0.0);  
				const int n = 10;  
				const GLfloat R = 5.0f;  
				const GLfloat pi = 3.1415926536f;  

				glBegin(GL_POLYGON);
				for (int i = 0; i < n; i++)  
				{  
					//GLfloat x = R*cos(2*pi/n*i) - windowWidth_ / 2 + g_mouse_curr_x; 
					//GLfloat y = R*sin(2*pi/n*i) + windowHeight_ / 2 - g_mouse_curr_y;
					cout<<"index = "<<g_selected_vertex_index<<endl;
					if(g_selected_vertex_index != 0){

						//GLfloat x = R*cos(2*pi/n*i) - windowWidth_ / 2 + g_model.trans_vertexes_[g_selected_vertex_index].x_; 
						//GLfloat y = R*sin(2*pi/n*i) + windowHeight_ / 2 - g_model.trans_vertexes_[g_selected_vertex_index].y_;

//						GLfloat x = R*cos(2*pi/n*i) + g_model.trans_vertexes_[g_selected_vertex_index].x_; 
	//					GLfloat y = R*sin(2*pi/n*i) + g_model.trans_vertexes_[g_selected_vertex_index].y_;
						GLfloat x = R*cos(2*pi/n*i) + g_model.trans_vertexes_[g_selected_vertex_index].x_ - windowWidth_/2; 
						GLfloat y = R*sin(2*pi/n*i) + windowHeight_/2-g_model.trans_vertexes_[g_selected_vertex_index].y_;
												//cout<<"oval x = "<<x <<" y = "<<y<<endl;

					//	cout<<"index.x = "<<g_model.trans_vertexes_[g_selected_vertex_index].x_ - windowWidth_/2<<" y = "<<windowHeight_/2-g_model.trans_vertexes_[g_selected_vertex_index].y_<<endl;

						glVertex2f(x, y);     
					}
				}  

				//cout<<"winhei = "<<windowHeight_<<" x = "<<windowWidth_<<endl;
				glEnd();
				///////// oval ///////////////

				cout<<"g_selected_vertex_index  = "<<g_selected_vertex_index <<endl;
		}
		//cout<<" "<<i<<" x = "<<g_model.trans_vertexes_[i].x_<<" y = "<<g_model.trans_vertexes_[i].y_<<endl;
	}

}

//����¼�����ס
void openglwindow::mousePressEvent(QMouseEvent *event){
	cout<<"press!!"<<endl;
	if(g_is_mouse_press == false){
		g_mouse_start_press_x = event->x();
		g_mouse_start_press_y = event->y();
		g_is_mouse_press = true;
	}
	cout<<"mouse curr x = "<<g_mouse_curr_x<<"		mouse curr y = "<<g_mouse_curr_y<<endl;
	//cout<<"mouse change x = "<<mouse_change_x<<"mouse change y = "<<mouse_change_y<<endl;

	update();
}

//����¼����ͷ�
void openglwindow::mouseReleaseEvent(QMouseEvent *event){
	cout<<"Release!!"<<endl;
	g_is_mouse_press = false;
}

//���б任���Գ�ʼ��
void openglwindow::transform_attribute_init(){
	g_model_altered.move_z = 0.0f;
	g_model_altered.rotate_theta = 0.0f;
	g_model_altered.scale = 1.0f;

	g_camera_altered.move_x = g_camera_altered.move_y = g_camera_altered.move_z = 0.0f;
	g_camera_altered.rotateX = g_camera_altered.rotateY = g_camera_altered.rotateZ = 0.0f;

	//drag_theta_x_ = 0.0f;
	//drag_theta_y_ = 0.0f;

	//�������룬���޸�
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
	drag_theta_z_ = 0.0f;
}



//������������mainwindow�ļ����¼�
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
	g_camera_altered.rotateX = camera_rotateX;	//��X����ת�� �Ƕ���
	g_camera_altered.rotateY = camera_rotateY;	//��Y����ת�� �Ƕ���       
	g_camera_altered.rotateZ = camera_rotateZ;


	//cout<<endl<<g_camera_altered.move_x<<endl;
	//cout<<g_camera_altered.move_y<<endl;
	//cout<<g_camera_altered.move_z<<endl;
	//cout<<g_camera_altered.rotateX<<endl;
	//cout<<g_camera_altered.rotateY<<endl;


	g_camera.total_rotate_theta_x_ += camera_rotateX;
	g_camera.total_rotate_theta_y_ += camera_rotateY;

}

//���ߺ���
void openglwindow::drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	glBegin(GL_LINES); ///������2����֮�仭һ������  
	glVertex2f(x1 - windowWidth_/2, -y1 + windowHeight_/2);  
	glVertex2f(x2 - windowWidth_/2, -y2 + windowHeight_/2); 
	glEnd(); //���� 
}


void openglwindow::initializeGL(){  
	setWindowSize(600,600);
	setGeometry(10, 10, windowWidth_, windowHeight_); 	//����widget������ͳߴ�  

	glClearColor(0.0, 0.0, 0.0, 0);//�������ʱ��ɫ  

	glClearDepth(1.0);//������Ȼ���  
	glEnable(GL_DEPTH_TEST);//������Ȳ���  
	glDepthFunc(GL_LEQUAL);//������Ȳ�������  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//����͸��У�� 
	rectX_ = 1.0;

	initCube();
	transform_attribute_init();
} 

void openglwindow::resizeGL(int w, int h){  
	glViewport(0,0,(GLsizei)w,(GLsizei)h);    	//�ӿڱ任  
	glMatrixMode(GL_PROJECTION);    	//ͶӰ�任  
	glLoadIdentity();  
	gluPerspective(90.0,(GLdouble)w/(GLdouble)h,0.1,10000.0);  
	//cout<<"glx = "<<(GLdouble)w<<" gly = "<<(GLdouble)h<<endl;

	//��ͼ�任  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();  
	gluLookAt(0.0,0.0,300,
		0.0,0.0,0.0,
		0.0,1.0,0.0);  
}  

//////////////////////////////////////////////////////
void openglwindow::paintGL(){  
	glClear(GL_COLOR_BUFFER_BIT);  	//����  




	glColor3f(255.0,255.0,255.0);  


	drawCube();
	transform_attribute_init();   //Ҫ������drawCube()

	drawLine(g_mouse_start_press_x, g_mouse_start_press_y, g_mouse_curr_x, g_mouse_curr_y);

	//glBegin(GL_POINTS); //���Ƶ�  
	//glVertex2f(0,0);  
	//glVertex2f(0.6,0.6); 
	//glEnd();  







	////////


	glFlush();  
	//glutSwapBuffers();
}  

//��ʼ��������
void openglwindow::initCube(){
	//g_objdata.setPath("D:\\cube.obj");
	//g_objdata.setPath("D:\\PolyDataWriter.obj");

	//g_objdata.setPath("D:\\bunny.obj");
	//g_objdata.setPath("F:\\source_code\\obj\\cube2.obj");
	//g_objdata.setPath("F:\\source_code\\obj\\wateringcan.obj");
	g_objdata.setPath("F:\\source_code\\obj\\hello.obj");

	//dodecahedron.obj

	g_objdata.readObjFile();

	int cube_vertex[24][3] = { 
		{-1,1,-1},{-1,-1,-1},{1,-1,-1},{1,1,-1},
		{1,1,-1},{1,-1,-1},{1,-1,1},{1,1,1},
		{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1},
		{-1,1,1},{-1,-1,1},{-1,-1,-1},{-1,1,-1},
		{-1,1,1},{-1,1,-1},{1,1,-1},{1,1,1},
		{-1,-1,-1},{-1,-1,1},{1,-1,1},{1,-1,-1}
	};


	////ȡ��ԭ����Ӳ�������
	//for (int i=0; i<24; ++i){   //�������ж���
	//	Vector3 v(cube_vertex[i][0],cube_vertex[i][1],cube_vertex[i][2]);
	//	g_model.local_vertexes_.push_back(v);            //�����ж��� ������model�� �ֲ�����
	//	g_model.trans_vertexes_.push_back(v+g_model.world_position_); //�����ж��� ������model�� ͸�����꣬----------trans_vertexes_ = �ֲ����� + ��������
	//	//g_model.save_vertexes_.push_back(v+g_model.world_position_); //�����������
	//}

	////����obj�ļ�����
	for (auto &v : g_objdata.obj_vertexes_){   //�������ж���
		g_model.local_vertexes_.push_back(v);            //�����ж��� ������model�� �ֲ�����
		g_model.trans_vertexes_.push_back(v+g_model.world_position_); //�����ж��� ������model�� ͸�����꣬----------trans_vertexes_ = �ֲ����� + ��������
	}


	// ģ�Ϳռ���ת
	//note:������ת���󣬷�����������
	Matrix model_rotateY_matrix;
	model_rotateY_matrix.setRotate(Vector3(0,1,0),45);
	Matrix model_rotateX_matrix;
	model_rotateX_matrix.setRotate(Vector3(1,0,0),45);
	Matrix model_rotateZ_matrix;
	model_rotateZ_matrix.setRotate(Vector3(0,0,1),45);

	Matrix matrix = model_rotateX_matrix * model_rotateY_matrix * model_rotateZ_matrix;

	int index2 = 0;
	for (auto &v : g_model.local_vertexes_){  //����model�� ���оֲ����� ����
		v = v * matrix;     //����ת��
		g_model.trans_vertexes_[index2++] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = �ֲ����� + ��������
		//g_model.trans_vertexes_[index2++].normal_ = v.normal_;   //������ʲô�ã�����

		//cout<<"v1 x = "<<g_model.trans_vertexes_[index2].x_<<" v1 y = "<<g_model.trans_vertexes_[index2].y_<<endl;
		//��һ��͸����������
	}

	//ȡ��ԭ����Ӳ�������
	//g_model.poly_indices_.push_back(TrangleIndex(0,1,2));
	//g_model.poly_indices_.push_back(TrangleIndex(2,3,0));  //front
	//g_model.poly_indices_.push_back(TrangleIndex(4,5,6));
	//g_model.poly_indices_.push_back(TrangleIndex(6,7,4));  //right
	//g_model.poly_indices_.push_back(TrangleIndex(8,9,10));
	//g_model.poly_indices_.push_back(TrangleIndex(10,11,8));  //back
	//g_model.poly_indices_.push_back(TrangleIndex(12,13,14));
	//g_model.poly_indices_.push_back(TrangleIndex(14,15,12));  //left
	//g_model.poly_indices_.push_back(TrangleIndex(16,17,18));
	//g_model.poly_indices_.push_back(TrangleIndex(18,19,16));  //top
	//g_model.poly_indices_.push_back(TrangleIndex(20,21,22));
	//g_model.poly_indices_.push_back(TrangleIndex(22,23,20));  //button

	for(auto &f : g_objdata.obj_triangles){
		g_model.poly_indices_.push_back(TrangleIndex(f.indices[0] - 1, f.indices[1] - 1, f.indices[2] - 1));
	}
}


void openglwindow::drawCube(){
	//4.1.1 ģ�Ϳռ䡾ƽ�ơ�
	Matrix model_move_matrix;    //����ƽ�ƾ���
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0,0,g_model_altered.move_z));    //-----> ��ϵ�����¼�
	g_model.world_position_ = g_model.world_position_ * model_move_matrix;     //��ƽ�ơ�

	//4.1.2 ģ�Ϳռ䡾��ת��
	Matrix model_rotate_matrix;    //������ת����
	model_rotate_matrix.setRotate(Vector3(0,1,0),g_model_altered.rotate_theta);   //-----> ��ϵ�����¼�

	//4.1.3 ģ�Ϳռ䡾���š�
	Matrix model_scale_matrix;        //�������ž���
	model_scale_matrix.setScale(Vector3(g_model_altered.scale,g_model_altered.scale,g_model_altered.scale));   //-----> ��ϵ�����¼�

	Matrix model_transform = model_rotate_matrix * model_scale_matrix;  //������ת���� + ���ž���

	//4.1.4 ת������������ϵ
	int index = 0;
	for (auto &v : g_model.local_vertexes_){    //����model�� ���оֲ����� ����
		v = v * model_transform;       //ģ�͡���ת�� + �����š� ���оֲ����� ����
		//������ת�����������š�����һ����� v = v
		g_model.trans_vertexes_[index] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = �ֲ����� + ��������
		//ԭ����������ĸ��ҳ�ʼ���� trans_vertexes_ 2017.8.12 21.55
		++index;
	}

	for (auto &v : g_model.trans_vertexes_){
		v *= save_matrix;
	}

	//������������
	g_camera.world_position_.x_ = 0;	
	g_camera.world_position_.y_ = 0;
	g_camera.world_position_.z_ = 0;

	g_camera.look_at_theta_ = Vector3(drag_theta_x_, drag_theta_y_,drag_theta_z_);  //�޸� ��� �Ƕ�  �������� z ����ת 20170813 11.44

	float dx, dy, dz;


	if(is_ctrl){    //����� ��z����תģʽ�Ļ�
		//��z����ת����   20170813  11.45
		//g_camera.view_transform_rotate(g_model.trans_vertexes_, 0, 0, 1, -drag_theta_y_);
		g_camera.view_transform_rotate(g_model.trans_vertexes_,save_matrix, 0, 0, 1, -drag_theta_y_);   //ʹ�� �б������ �����ذ汾

	}
	else{
		//��ƽ�ơ���������y�᡿
		if(can_x_){
			Matrix camera_translation_matrix_x;
			camera_translation_matrix_x.identify();
			dx = g_camera.dis_camera_model_ * Sin(drag_theta_x_);
			dz = g_camera.dis_camera_model_ * (1 - Cos(drag_theta_x_));
			camera_translation_matrix_x.setTranslation(Vector3(dx, 0, dz));
			g_camera.set_position(camera_translation_matrix_x);   //��ƽ�ơ��������������

			//g_camera.view_transform_translation(g_model.trans_vertexes_);  // ��������������� ������ ƽ�Ƶ� trans_vertexes_
			//g_camera.view_transform_rotate(g_model.trans_vertexes_, 0, 1, 0, g_camera.look_at_theta_.x_);  //����y����ת

			g_camera.view_transform_translation(g_model.trans_vertexes_, save_matrix);  //ʹ�� �б������ �����ذ汾
			g_camera.view_transform_rotate(g_model.trans_vertexes_, save_matrix, 0, 1, 0, g_camera.look_at_theta_.x_);   //ʹ�� �б������ �����ذ汾

		}

		//������������
		g_camera.world_position_.x_ = 0;	
		g_camera.world_position_.y_ = 0;
		g_camera.world_position_.z_ = 0;

		//��ƽ�ơ� ���� ����x�᡿
		if(can_y_){
			Matrix camera_translation_matrix_y;
			camera_translation_matrix_y.identify();
			dy = g_camera.dis_camera_model_ * Sin(drag_theta_y_);
			dz = g_camera.dis_camera_model_ * (1 - Cos(drag_theta_y_));
			camera_translation_matrix_y.setTranslation(Vector3(0, dy, dz));
			g_camera.set_position(camera_translation_matrix_y);   //��ƽ�ơ�

			//g_camera.view_transform_translation(g_model.trans_vertexes_);  //��ƽ�ơ�һ��
			//g_camera.view_transform_rotate(g_model.trans_vertexes_, 1, 0, 0, -g_camera.look_at_theta_.y_);  //��x����ת

			g_camera.view_transform_translation(g_model.trans_vertexes_, save_matrix);  //ʹ�� �б������ �����ذ汾
			g_camera.view_transform_rotate(g_model.trans_vertexes_, save_matrix, 1, 0, 0, -g_camera.look_at_theta_.y_);  //ʹ�� �б������ �����ذ汾
		}
	}


	//g_model.save_vertexes_ = g_model.trans_vertexes_;   //�������������ñ任������� trans_vertexes

	// 4.3.3 ͸�ӳ���
	for (int i=0;i<g_model.trans_vertexes_.size();++i){
		//cout<<i<<" w_ =  "<<g_model.trans_vertexes_[i].w_<<endl;
		g_model.trans_vertexes_[i].x_ = g_model.trans_vertexes_[i].x_/g_model.trans_vertexes_[i].z_;  //��ĸ��ʵ�� ����ԶС
		g_model.trans_vertexes_[i].y_ = g_model.trans_vertexes_[i].y_/g_model.trans_vertexes_[i].z_; 
		//model.trans_vertexes_[i].position_.z_ = model.trans_vertexes_[i].position_.z_/model.trans_vertexes_[i].position_.w_; 
		//model.trans_vertexes_[i].position_.w_ = 1/model.trans_vertexes_[i].position_.w_;
	}

	/************************************************************************/
	/* 4.4  ���ڱ任                                                         */
	/************************************************************************/
	int half_width = windowWidth_/2;
	int half_height = windowHeight_/2;
	for (int i=0;i<g_model.trans_vertexes_.size();++i){
		g_model.trans_vertexes_[i].x_ *= half_width;    //�������Ŵ�
		g_model.trans_vertexes_[i].y_ *= half_height;   //�������Ŵ�
		g_model.trans_vertexes_[i].x_ += half_width;    //ʹ��ģ��λ����Ļ���м�
		g_model.trans_vertexes_[i].y_ = half_height - g_model.trans_vertexes_[i].y_; //ʹ��ģ��λ����Ļ���м�
	}

	//�����߿�ģ��
	drawWireframeModel(g_model);
	if(is_view_mode_ == false){
		is_view_mode_ = true;
	}


	////��ѡ�㡿���������е�
	//int g_selected_vertex_index = 0;
	//for (int i=0; i<g_model.trans_vertexes_.size(); ++i){
	//	if(
	//		(
	//		(g_mouse_curr_x >= (g_model.trans_vertexes_[i].x_ - 1)) 
	//		&&
	//		(g_mouse_curr_x <= (g_model.trans_vertexes_[i].x_ + 1)) 
	//		)

	//		&& 	
	//		(
	//		(g_mouse_curr_y >= (g_model.trans_vertexes_[i].y_ -1))
	//		&&
	//		(g_mouse_curr_y <= (g_model.trans_vertexes_[i].y_ + 1))
	//		)
	//		){
	//			g_selected_vertex_index = i;
	//			cout<<"g_selected_vertex_index  = "<<g_selected_vertex_index <<endl;
	//	}
	//	//cout<<" "<<i<<" x = "<<g_model.trans_vertexes_[i].x_<<" y = "<<g_model.trans_vertexes_[i].y_<<endl;
	//}



}

void openglwindow::drawWireframeModel(Model & model){
	for (int index=0; index<model.poly_indices_.size(); ++index){  //��������������
		//note:���������ε������㡪����������trans_vertexes_��ȡ���㡿

		Vector3 v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vector3 v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vector3 v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];

		//���������ε�������
		drawLine(v1.x_,v1.y_,v2.x_,v2.y_);
		drawLine(v3.x_,v3.y_,v2.x_,v2.y_);
		drawLine(v1.x_,v1.y_,v3.x_,v3.y_);
		//cout<<"v1 x = "<<v1.x_<<" v1 y = "<< v1.y_<<endl;
		//cout<<"v2 x = "<<v2.x_<<" v2 y = "<< v2.y_<<endl;
		//cout<<"v3 x = "<<v3.x_<<" v3 y = "<< v3.y_<<endl;
	}
}
