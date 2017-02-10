#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <SOIL.h>
#include <glm/glm.hpp>


#ifndef SCENE_H
#define SCENE_H
using namespace std;

struct Wall {
	float x;
	float y;
	float length;
	float width;

	bool isPointInside(glm::vec3 p);
	Wall(int _x, int _y, float _l, float _w);
	Wall();
	~Wall();
};

struct Scene
{
	int walls[100][200];
	int width;
	int height;
	int numberOfTextures =0;
	GLuint Textures[20];
	string texturePaths[10];
	int cellSize;
	vector<GLfloat> vertices[20];
	vector<GLfloat> collider;
	int wallHeight =1;
	vector<Wall> Cwalls;
	



	void LoadLevel(const string &levelstring);
	
	void GenerateMesh();
	void GenerateTextures();
	Scene(int w, int h);
	~Scene();
};








#endif // !SCENE_H
