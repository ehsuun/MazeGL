#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include <iomanip>


#ifndef SCENE_H
#define SCENE_H
using namespace std;


struct Scene
{
	int** walls;
	int width;
	int height;

	void LoadLevel(string levelstring);

	Scene(int w, int h);
	~Scene();
};







#endif // !SCENE_H
