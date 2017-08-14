#ifndef OBJDATA_H_
#define OBJDATA_H_

#include "Vector3.h"
#include <vector>
#include <string>
#include <iostream>


#include<fstream>
#include <sstream>    //istringstream

using namespace std;

class TriangleIndex        //下标 ———— 本质上是一个 有三个元素的一维数组
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


	ifstream in_;  //用于读入obj文件
	string filepath_;
	string str_line_;   //用于临时保存 一行 的字符 

	//obj数据
	vector<Vector3> obj_vertexes_;  //obj顶点数据集合
	vector<TriangleIndex> obj_triangles;   //obj三角形集合

	//函数
	void readObjFile();  //读取文件
	void printObj();
	void setPath(string path){filepath_ = path;}
};
#endif