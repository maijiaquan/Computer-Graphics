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

#include "Model.h"
#include "Camera.h"
#include "Math.h"

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


	//绘制直线
	void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);


	//绘制线框模型
	void drawWireframeModel(Model & model);

	//鼠标事件
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	//键盘事件
	//void keyPressEvent(QKeyEvent *event);


	//画图函数
	void drawCube();   //画正方体
	void initCube();    //正方体初始化

	//void transform_attribute_init();


	//关联mainwindow键盘事件
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

	float drag_theta_x_;   //表示鼠标拖拽后，相机旋转的角度 x轴
	float drag_theta_y_;   //表示鼠标拖拽后，相机旋转的角度 y轴

	bool can_x_;
	bool can_y_;

	bool is_view_mode_;   //浏览模式

	//所有变换属性初始化
	void transform_attribute_init();
	
	
	
	void changeAttribute();

private:
	int windowWidth_;
	int windowHeight_;

protected:
    void timerEvent(QTimerEvent *event);
};



#endif // OPENGLWINDOW_H
