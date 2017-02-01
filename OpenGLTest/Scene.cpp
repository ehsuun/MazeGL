#include "Scene.h"


std::string trim(const std::string& str)
{
	static const std::string ws = " \t\n";
	auto first = str.find_first_not_of(ws);
	auto last = str.find_last_not_of(ws);
	return first == std::string::npos ? "" : str.substr(first, last - first + 1);
}

void Scene::LoadLevel(const string &levelstring)
{
	stringstream st = stringstream(trim(levelstring));
	string line;
	while (std::getline(st, line))
	{
		istringstream iss(line);

		string result;
		cout << line.find("CELL");
	}



	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++)
		{
			//cout << walls[i][j]<<"\n";
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
