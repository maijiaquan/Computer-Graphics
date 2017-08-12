#include "openglwindow.h"


//�ع����㷨�汾
int g_ratio = 5;
int g_translation = 17;

int g_mouse_start_press_x = 0;
int g_mouse_start_press_y = 0;

int g_mouse_curr_x = 0;
int g_mouse_curr_y = 0;

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
	float rotateX;	//�����X����ת�� �Ƕ���
	float rotateY;	//�����Y����ת�� �Ƕ���
	float rotateZ;
}g_camera_altered;



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

	startTimer(1000);    //���ü��ʱ��Ϊ1000ms���ú�������ֵΪ1
	//startTimer(3000);  //����ж����ʱ�����ڼ�����ʱ���ķ���ֵ���Ǽ�
}

openglwindow::~openglwindow(){ }

//��ʱ������
void openglwindow::timerEvent(QTimerEvent *t)//��ʱ��ʱ��
{
	//g_model_altered.rotate_theta = 0.5f;
	//update();

	g_camera.printCamereInfo();
	g_model.printModelInfo();

	cout<<"cal dis = "<<calculateDistance(g_model.world_position_.x_,
		g_model.world_position_.y_,
		g_model.world_position_.z_,
		g_camera.world_position_.x_,
		g_camera.world_position_.y_,
		g_camera.world_position_.z_)<<endl;
}


void openglwindow::mouseMoveEvent(QMouseEvent *e){
	g_mouse_curr_x = e->x();
	g_mouse_curr_y = e->y();
	//cout<<"x = "<<g_mouse_curr_x<<" y = "<<g_mouse_curr_y <<endl;

	if(g_is_mouse_press){
		update();
	}
}

void openglwindow::mousePressEvent(QMouseEvent *event){
	//canDrag = true;
	cout<<"press!!"<<endl;
	if(g_is_mouse_press == false){
		g_mouse_start_press_x = event->x();
		g_mouse_start_press_y = event->y();
		g_is_mouse_press = true;
	}
	update();
}

void openglwindow::mouseReleaseEvent(QMouseEvent *event){
	//canDrag = false;
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

	//glVertex2f(x1 - windowWidth_/2, -(y1 - windowHeight_/2));  
	//glVertex2f(x2 - windowWidth_/2, -(y2 - windowHeight_/2)); 

	//cout<<windowWidth_<<endl;
	glVertex2f(x1 - windowWidth_/2, -y1 + windowHeight_/2);  
	glVertex2f(x2 - windowWidth_/2, -y2 + windowHeight_/2); 

	//glVertex2f(x1, -y1);  
	//glVertex2f(x2, -y2);  

	//glVertex2f(x1/g_ratio - g_translation, -y1/g_ratio + g_translation);  
	//glVertex2f(x2/g_ratio - g_translation, -y2/g_ratio + g_translation);  
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
	cout<<"glx = "<<(GLdouble)w<<" gly = "<<(GLdouble)h<<endl;

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


	//drawLine(0.0F, 0.0f, rectX_, rectX_);

	drawCube();
	transform_attribute_init();   //Ҫ������drawCube()



	drawLine(g_mouse_start_press_x, g_mouse_start_press_y, g_mouse_curr_x, g_mouse_curr_y);
	glBegin(GL_POINTS); //���Ƶ�  
	glVertex2f(0,0);  
	glVertex2f(0.6,0.6); 


	glEnd();  
	glFlush();  




	//glutSwapBuffers();


}  

void openglwindow::initCube(){
	int cube_vertex[24][3] = { 
		{-1,1,-1},{-1,-1,-1},{1,-1,-1},{1,1,-1},
		{1,1,-1},{1,-1,-1},{1,-1,1},{1,1,1},
		{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1},
		{-1,1,1},{-1,-1,1},{-1,-1,-1},{-1,1,-1},
		{-1,1,1},{-1,1,-1},{1,1,-1},{1,1,1},
		{-1,-1,-1},{-1,-1,1},{1,-1,1},{1,-1,-1}
	};


	for (int i=0; i<24; ++i){   //�������ж���
		Vector3 v(cube_vertex[i][0],cube_vertex[i][1],cube_vertex[i][2]);
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
		g_model.trans_vertexes_[index2] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = �ֲ����� + ��������
		//g_model.trans_vertexes_[index2++].normal_ = v.normal_;   //������ʲô�ã�����


		//cout<<"v1 x = "<<g_model.trans_vertexes_[index2].x_<<" v1 y = "<<g_model.trans_vertexes_[index2].y_<<endl;
		//��һ��͸����������
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
	//4.1.1 ģ�Ϳռ�ƽ��
	Matrix model_move_matrix;    //����ƽ�ƾ���
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0,0,g_model_altered.move_z));    //-----> ��ϵ�����¼�
	g_model.world_position_ = g_model.world_position_ * model_move_matrix;     //��ƽ�ơ�

	//4.1.2 ģ�Ϳռ���ת
	Matrix model_rotate_matrix;    //������ת����
	model_rotate_matrix.setRotate(Vector3(0,1,0),g_model_altered.rotate_theta);   //-----> ��ϵ�����¼�

	//4.1.3 ģ�Ϳռ�����
	Matrix model_scale_matrix;        //�������ž���
	model_scale_matrix.setScale(Vector3(g_model_altered.scale,g_model_altered.scale,g_model_altered.scale));   //-----> ��ϵ�����¼�

	Matrix model_transform = model_rotate_matrix * model_scale_matrix;  //������ת���� + ���ž���














	//��һ��
	//camera_move_matrix.identify();
	////4.2.2 �������ת��  -----> ��ϵ�����¼�
	//g_camera.set_lookAt(Vector3(g_camera_altered.rotateX, g_camera_altered.rotateY, 0)); //look_at_theta_ + ��ת���������� x �� y����

	//float dx = g_camera.dis_camera_model_ * Sin(g_camera.total_rotate_theta_x_) - g_camera.last_dx;
	//float dz = g_camera.dis_camera_model_ * (1 - Cos(g_camera.total_rotate_theta_y_)) - g_camera.last_dz;

	//g_camera.last_dx = dx;
	//g_camera.last_dz = dz;
	//
	//camera_move_matrix.setTranslation(Vector3(dx, 0, dz));
	//	g_camera.set_position(camera_move_matrix);  //	//�ȡ���ת������ƽ�ơ����Ӷ����������������ת

	//cout<<"Sin(g_camera_altered.rotateX) = "<<Sin(g_camera_altered.rotateX)<<endl;
	//cout<<"Cos(g_camera_altered.rotateX)"<<Cos(g_camera_altered.rotateX)<<endl;


	//�������֪��Ϊʲô����ᷢ���仯��Ӧ���ǹ�ʽ����  2017.8.9  19��31
	//cout<<"drag_theta_x_"<<drag_theta_x_<<endl;
	//cout<<"drag_theta_y_"<<drag_theta_y_<<endl;

	//������
	//������������
	g_camera.world_position_.x_ = 0;	
	g_camera.world_position_.y_ = 0;
	g_camera.world_position_.z_ = 0;
	//���ù۲�Ƕ�
	g_camera.look_at_theta_ = Vector3(0,0,0);
	
	//g_camera.view_transform(g_model.trans_vertexes_);  //���¸��� model�� trans_vertexes_ 

	g_camera.set_lookAt(Vector3(drag_theta_x_, drag_theta_y_, 0)); //look_at_theta_ + ��ת���������� x �� y����


	//	//4.2.1 ������ƶ���  -----> ��ϵ�����¼�
	//Matrix camera_move_matrix;   //���� ƽ�ƾ���
	//camera_move_matrix.identify();
	//camera_move_matrix.setTranslation(Vector3(g_camera_altered.move_x,g_camera_altered.move_y,g_camera_altered.move_z));
	//g_camera.set_position(camera_move_matrix);  //���� ƽ�ƾ���

	//��ƽ�ơ�
	float dx, dy, dz;
	//��ƽ�ơ���������y�᡿
	Matrix camera_translation_matrix_x;
	camera_translation_matrix_x.identify();
	dx = g_camera.dis_camera_model_ * Sin(drag_theta_x_);
	dz = g_camera.dis_camera_model_ * (1 - Cos(drag_theta_x_));
	camera_translation_matrix_x.setTranslation(Vector3(dx, 0, dz));
	g_camera.set_position(camera_translation_matrix_x);   //��ƽ�ơ�

	//��ƽ�ơ� ���� ����x�᡿
	//Matrix camera_translation_matrix_y;
	//camera_translation_matrix_y.identify();
	//dy = g_camera.dis_camera_model_ * Sin(-drag_theta_y_);
	//dz = g_camera.dis_camera_model_ * (1 - Cos(-drag_theta_y_));
	//camera_translation_matrix_y.setTranslation(Vector3(0, dy, dz));
	//g_camera.set_position(camera_translation_matrix_y);   

	//�� = drag_theta_y_
	//�ڶ��Ρ�ƽ�ơ�

	//Vector3 normal_vector;
	if(drag_theta_y_){
		Matrix camera_translation_matrix_y;
		camera_translation_matrix_y.identify();
		dy = g_camera.dis_camera_model_ * Sin(drag_theta_y_);
		float length = g_camera.dis_camera_model_ * Cos(drag_theta_y_); 
		cout<<"length =	"<<length<<endl;
		float gamma = g_camera.dis_camera_model_ - length;
		cout<<"gamma = "<<gamma<<endl;
		dx = - gamma * Sin(drag_theta_x_);
		cout<<"dx = "<<dx<<endl;
		dz = gamma * Cos(drag_theta_x_);
		cout<<"dy = "<<dy<<endl;
		camera_translation_matrix_y.setTranslation(Vector3(dx,dy,dz));
		g_camera.set_position(camera_translation_matrix_y);

		////���㷨����
		//normal_vector.x_ = g_camera.world_position_.y_*g_camera.world_position_.z_;
		//normal_vector.y_ = 0;
		//normal_vector.z_ = -g_camera.world_position_.x_ * g_camera.world_position_.y_;
		////ת���ɵ�λ����
		//float normal_vector_norm = sqrt(normal_vector.x_*normal_vector.x_ 
		//	+ normal_vector.y_*normal_vector.y_
		//	+ normal_vector.z_*normal_vector.z_);
		//normal_vector.x_ /= normal_vector_norm;  
		//normal_vector.y_ /= normal_vector_norm;  
		//normal_vector.z_ /= normal_vector_norm;  


		//cout<<"nv x = "<<normal_vector.x_<<endl;
		//cout<<"nv y = "<<normal_vector.y_<<endl;
		//cout<<"nv z = "<<normal_vector.z_<<endl;
	}



	//4.1.4 ת������������ϵ
	int index = 0;
	for (auto &v : g_model.local_vertexes_){    //����model�� ���оֲ����� ����
		v = v * model_transform;       //ģ�͡���ת�� + �����š� ���оֲ����� ����
		g_model.trans_vertexes_[index] = v + g_model.world_position_;  //---------------------------trans_vertexes_ = �ֲ����� + ��������
		++index;
	}

	//4.2.3 ת�����������
	//g_camera.view_transform(g_model.trans_vertexes_);  //���¸��� model�� trans_vertexes_ 
	g_camera.view_transform_translation(g_model.trans_vertexes_);  //��ƽ�ơ�һ��
	g_camera.view_transform_rotate(g_model.trans_vertexes_, 0, 1, 0, g_camera.look_at_theta_.x_);  //����y����ת
	if(drag_theta_y_){
		g_camera.view_transform_rotate(g_model.trans_vertexes_, 
			//-normal_vector.x_, 
			//-normal_vector.y_, 
			//-normal_vector.z_,
			1,0,0,
			-drag_theta_y_); 
	}

	//



	// 4.3.3 ͸�ӳ���
	for (int i=0;i<g_model.trans_vertexes_.size();++i){
		//cout<<i<<" w_ =  "<<g_model.trans_vertexes_[i].w_<<endl;
		g_model.trans_vertexes_[i].x_ = g_model.trans_vertexes_[i].x_/g_model.trans_vertexes_[i].z_; 
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
		//cout<<"v1 x = "<<g_model.trans_vertexes_[i].x_<<" v1 y = "<<g_model.trans_vertexes_[i].y_<<endl;

		g_model.trans_vertexes_[i].x_ *= half_width;    //�������Ŵ�
		g_model.trans_vertexes_[i].y_ *= half_height;   //�������Ŵ�
		g_model.trans_vertexes_[i].x_ += half_width;    //ʹ��ģ��λ����Ļ���м�
		g_model.trans_vertexes_[i].y_ = half_height - g_model.trans_vertexes_[i].y_; //ʹ��ģ��λ����Ļ���м�
	}

	//�����߿�ģ��
	drawWireframeModel(g_model);


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
