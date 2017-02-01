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
	int** walls;
	int width;
	int height;
	int numberOfTextures;
	vector<string> texturePaths;
	int cellSize;
	
	int wallHeight =1;



	void LoadLevel(const string &levelstring);

	Scene(int w, int h);
	~Scene();
};







#endif // !SCENE_H
