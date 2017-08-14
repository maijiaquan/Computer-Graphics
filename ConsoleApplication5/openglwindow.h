#pragma once
#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>  
#include<GL\GLU.h>
#include<iostream>
#include<GL\glut.h>
#include <QMouseEvent>
//#include<QKeyEvent>
//#include <Windows.h>

#include "objdata.h"
#include "Model.h"
#include "Camera.h"
#include "Math.h"
#include <string>
using namespace std;

#define MAX_CHAR       128




class openglwindow : public QOpenGLWidget
{
	Q_OBJECT

public:
	openglwindow(QWidget *parent);
	~openglwindow();

	void initializeGL();  
	void resizeGL(int width, int height);  
	void paintGL();  

	GLfloat rectX_;

	void setWindowSize(int width, int height){
		windowWidth_ = width;
		windowHeight_ = height;
	}


	//����ֱ��
	void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);


	//�����߿�ģ��
	void drawWireframeModel(Model & model);

	//����¼�
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	//�����¼�
	//void keyPressEvent(QKeyEvent *event);


	//��ͼ����
	void drawCube();   //��������
	void initCube();    //�������ʼ��

	//void transform_attribute_init();


	//����mainwindow�����¼�
	float model_move_z;
	float model_rotate_theta;
	float model_scale;

	float camera_move_z;
	float camera_rotate_theta;
	float camera_scale;
	float camera_move_x;
	float camera_move_y;
	float camera_rotateX;
	float camera_rotateY;
	float camera_rotateZ;

	float drag_theta_x_;   //��ʾ�����ק�������ת�ĽǶ� x��
	float drag_theta_y_;   //��ʾ�����ק�������ת�ĽǶ� y��
	float drag_theta_z_;   //��ʾ�����ק�������ת�ĽǶ� z��

	bool can_x_;
	bool can_y_;

	bool is_view_mode_;   //���ģʽ
	bool is_ctrl;       //ctrl���Ƿ���

	Matrix save_matrix;

	//���б任���Գ�ʼ��
	void transform_attribute_init();
	void changeAttribute();

private:
	int windowWidth_;
	int windowHeight_;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // OPENGLWINDOW_H
