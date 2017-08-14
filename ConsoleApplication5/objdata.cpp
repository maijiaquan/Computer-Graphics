#include "objdata.h"

void ObjData::readObjFile(){
	in_.open(filepath_);

	while(getline(in_, str_line_)){ //遍历每一行
		//cout<<"str_line_ = "<<str_line_<<endl;
		istringstream in(str_line_); //获得该行
		string str1, str2, str3, str4;
		in>>str1>>str2>>str3>>str4; 

		float i = 0, j = 0, k = 0;
		i = atof(str2.c_str());
		j = atof(str3.c_str());
		k = atof(str4.c_str());

		if(str_line_[0] == 'v'){
			Vector3 temp_v(i, j, k);
			//cout<<" v "<<i<<" "<<j<<" "<<k<<endl;
			obj_vertexes_.push_back(temp_v);
		}

		if(str_line_[0] == 'f'){
			TriangleIndex temp_t(i, j, k);
			//cout<<" f "<<i<<" "<<j<<" "<<k<<endl;
			obj_triangles.push_back(temp_t);
		}
	}
}

void ObjData::printObj(){
	cout<<"-------  vertexes -------"<<endl;
	for(auto &v : obj_vertexes_){
		cout<<" x = "<<v.x_<<" y = "<<v.y_<<" z = "<<v.z_<<endl;
	}

	cout<<"-------  faces -------"<<endl;
	for(auto &f : obj_triangles){
		cout<<" x = "<<f.indices[0]<<" y = "<<f.indices[1]<<" z = "<<f.indices[2]<<endl;
	}
}