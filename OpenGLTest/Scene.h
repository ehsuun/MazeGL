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
	int walls[20][40];
	int width;
	int height;
	int numberOfTextures =0;
	string texturePaths[10];
	int cellSize;
	
	int wallHeight =1;



	void LoadLevel(const string &levelstring);

	Scene(int w, int h);
	~Scene();
};







#endif // !SCENE_H
