#include "Scene.h"

std::string trim(const std::string& str)
{
	static const std::string ws = " \t\n";
	auto first = str.find_first_not_of(ws);
	auto last = str.find_last_not_of(ws);
	return first == std::string::npos ? "" : str.substr(first, last - first + 1);
}

std::string deleteComments(const std::string& str)
{
	stringstream st = stringstream(str);
	string line;
	string output;
	int l = 0;
	while (std::getline(st, line))
	{
		// add new line skip the frist line
		if (l > 0) {
			output.append("\n");
		}		
		l++;
		string::size_type loc;
		loc = line.find('#', 0);
		bool exists = (loc != string::npos);
		if (exists) {
			if (loc != 0) {
				string clean = line.substr(0, loc);
				output.append(clean);
			}
		// we skip the line if # is at 0
		}
		else {
			output.append(line);
		}

	}
	return output;
}

void Scene::LoadLevel(const string &levelstring)
{
	stringstream st = stringstream(deleteComments(trim(levelstring)));
	string line;
	string token ;

	while (std::getline(st, line))
	{
		istringstream iss(line);
		string::size_type loc;
		string result;
		bool exists;

		//TEXTURE
		token = "TEXTURES";

		loc = line.find(token, 0);
		exists = (loc != string::npos);
		if (exists) {
			string value = line.substr(loc + token.length(), 1000);
			numberOfTextures = stoi(value);
			cout << " number of tex: " << numberOfTextures;

			for (int i = 0; i < numberOfTextures; i++) {
				std::getline(st, line);
				texturePaths[i] = line.substr(line.find(' ', 0) + 1, 1000);
				cout << " texture " << i << " : " << texturePaths[i];
			}
		}



		//CELL
		token = "CELL";

		loc = line.find(token, 0);
		exists = (loc != string::npos);
		if (exists) {
			string value = line.substr(loc+ token.length(), 1000);
			cellSize = stoi(value);

			cout <<" cell size: " <<cellSize;
		}
		//HEIGHT
		token = "HEIGHT";

		loc = line.find(token, 0);
		exists = (loc != string::npos);
		if (exists) {
			string value = line.substr(loc + token.length(), 1000);
			wallHeight = stoi(value);
			cout << " wall height: " << wallHeight;
		}

		//DIMENSIONS
		token = "DIMENSIONS";

		loc = line.find(token, 0);
		exists = (loc != string::npos);
		if (exists) {
			string value = line.substr(loc + token.length(), line.find(' ',0));
			width = stoi(value);
			value = line.substr(line.find(' ', line.find(' ', 0)+1) + 1, 1000);
			height = stoi(value);
			cout << " width: " << width << " height : "<<height;
		}



		//FLOORPLAN
		token = "FLOORPLAN";
		
		loc = line.find(token, 0);
		exists = (loc != string::npos);
		if (exists) {
			for (int j = 0; j < height; j++) {
				cout << "\n";
				std::getline(st, line);
				for (int i = 0; i < width; i++) {
					while (line.substr(0, 1) == " ") {
						line = line.erase(0, 1);
					}
					walls[i][j * 2] = stoi(line.substr(0,1));
					line = line.erase(0, 1);
					cout << walls[i][j * 2] << " ";
				}
				cout << "\n";
				std::getline(st, line);
				for (int i = 0; i < width; i++) {
					while (line.substr(0, 1) == " ") {
						line = line.erase(0, 1);
					}
					walls[i][j * 2+1] = stoi(line.substr(0, 1));
					line = line.erase(0, 1);
					cout << walls[i][j * 2+1] << " ";
				}
				
			}
		}

	}



	

}

Scene::Scene(int w, int h)
{
	width = w;
	height = h;
	/*
	walls = new int*[width];
	for (int i = 0; i<width; i++)
	{
		walls[i] = new int[height*2];
		for (int j = 0; j < height*2; j++) {
			walls[i][j] = 0;
		}
	}
	*/
}

Scene::~Scene()
{
	for (int i = 0; i < width; ++i) {
		delete[] walls[i];
	}
	delete[] walls;
}
