#include "Scene.h"

void Scene::LoadLevel(string levelstring)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
		{
			cout << walls[i][j]<<"\n";
		}
	}
}

Scene::Scene(int w, int h)
{
	width = w;
	height = h;
	walls = new int*[width];
	for (int i = 0; i<width; i++)
	{
		walls[i] = new int[height];
		for (int j = 0; j < height; j++) {
			walls[i][j] = i*j;
		}
	}
}

Scene::~Scene()
{
	for (int i = 0; i < width; ++i) {
		delete[] walls[i];
	}
	delete[] walls;
}
