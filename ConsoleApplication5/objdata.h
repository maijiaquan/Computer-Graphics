#ifndef OBJDATA_H_
#define OBJDATA_H_

#include "Vector3.h"
#include <vector>
#include <string>
#include <iostream>


#include<fstream>
#include <sstream>    //istringstream

using namespace std;

class TriangleIndex        //�±� �������� ��������һ�� ������Ԫ�ص�һά����
{
public:
	int indices[3];
	TriangleIndex(int i,int j,int k){
		indices[0] = i;
		indices[1] = j;
		indices[2] = k;
	}
	TriangleIndex(){}
	
	
};


class ObjData{
public:
	ObjData(){}
	ObjData(string path):
		filepath_(path)
	{}


	ifstream in_;  //���ڶ���obj�ļ�
	string filepath_;
	string str_line_;   //������ʱ���� һ�� ���ַ� 

	//obj����
	vector<Vector3> obj_vertexes_;  //obj�������ݼ���
	vector<TriangleIndex> obj_triangles;   //obj�����μ���

	//����
	void readObjFile();  //��ȡ�ļ�
	void printObj();
	void setPath(string path){filepath_ = path;}
};
#endif