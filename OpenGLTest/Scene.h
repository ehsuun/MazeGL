#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>


#ifndef SCENE_H
#define SCENE_H
using namespace std;


struct Scene
{
	int walls[100][200];
	int width;
	int height;
	int numberOfTextures =0;
	string texturePaths[10];
	int cellSize;
	vector<GLfloat> vertices;
	int wallHeight =1;



	void LoadLevel(const string &levelstring);
	
	void GenerateMesh();

	Scene(int w, int h);
	~Scene();
};







#endif // !SCENE_H
