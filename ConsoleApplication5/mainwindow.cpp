#include "mainwindow.h"

float a;
mainwindow::mainwindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.widget->paintGL();

}

mainwindow::~mainwindow()
{

}


//���̲���
void mainwindow::keyPressEvent(QKeyEvent *event){
	int keyValue = event->key();


	switch (keyValue){
		//���̲���������ģ��
	case Qt::Key_Up:	ui.widget->model_move_z = 0.1f;	break;  //ƽ������
	case Qt::Key_Down:	ui.widget->model_move_z = -0.1f;	break;

	case Qt::Key_Left:	ui.widget->model_rotate_theta = 1.0f;	break; 		//��ת����
	case Qt::Key_Right:	ui.widget->model_rotate_theta = -1.0f;	break;    //3.5

	case Qt::Key_8:	ui.widget->model_scale = 1.01;	break; //��������
	case Qt::Key_2:	ui.widget->model_scale = 0.99;	break;


		//���̲��������
	case Qt::Key_A:	ui.widget->camera_move_x = -0.1f;	break; //ƽ�����
	case Qt::Key_D:	ui.widget->camera_move_x = 0.1;	break;
	case Qt::Key_W:	ui.widget->camera_move_y = 0.1f;	break;
	case Qt::Key_S:	ui.widget->camera_move_y = -0.1f;	break;
	case Qt::Key_Q:	ui.widget->camera_move_z = 0.1;	break;
	case Qt::Key_E:	ui.widget->camera_move_z = -0.1;	break;

		//case Qt::Key_I: ui.widget->camera_rotateY = 1; break;
		//case Qt::Key_K: ui.widget->camera_rotateY = -1; break;
		//case Qt::Key_J: ui.widget->camera_rotateX = 1; break;
		//case Qt::Key_L: ui.widget->camera_rotateX = -1; break;


		//case Qt::Key_J: ui.widget->drag_theta_x_ += 1; break;
		//case Qt::Key_L: ui.widget->drag_theta_x_ -= 1; break;

		//case Qt::Key_I: ui.widget->drag_theta_y_ += 1; break;
		//case Qt::Key_K: ui.widget->drag_theta_y_ -= 1; break;

	case Qt::Key_J: ui.widget->drag_theta_x_ = 1; break;
	case Qt::Key_L: ui.widget->drag_theta_x_ = -1; break;

	case Qt::Key_I: ui.widget->drag_theta_y_ = 1; break;
	case Qt::Key_K: ui.widget->drag_theta_y_ = -1; break;
	case Qt::Key_1: 
		{
			if(ui.widget->can_x_ == false){
				ui.widget->can_x_ = true;
			}else{
				ui.widget->can_x_ = false;
			}
			cout<<"can x ="<<ui.widget->can_x_<<endl;
		}
		break;
	case Qt::Key_0: 
		{
			if(ui.widget->can_y_ == false){
				ui.widget->can_y_ = true;
			}else{
				ui.widget->can_y_ = false;
			}
			cout<<"can y ="<<ui.widget->can_y_<<endl;
		}
		break;
	case Qt::Key_3: 
		{
			if(ui.widget->is_view_mode_ == false){
				ui.widget->is_view_mode_ = true;
			}else{
				ui.widget->is_view_mode_ = false;
			}
			cout<<"is_view_mode_ ="<<ui.widget->is_view_mode_<<endl;
		}
		break;
	default:	break;
	}

	ui.widget->changeAttribute();
	ui.widget->update();
}


