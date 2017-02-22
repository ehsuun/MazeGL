#include "Scene.h"

const float WALLTHICKNESS = 1.0f;

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
			
		}		
		l++;
		string::size_type loc;
		loc = line.find('#', 0);
		bool exists = (loc != string::npos);
		if (exists) {
			if (loc != 0) {
				string clean = line.substr(0, loc);
				output.append(clean);
				if(clean!="")
				output.append("\n");
			}
		// we skip the line if # is at 0
		}
		else {
			output.append(line);
			if (line != "")
			output.append("\n");
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

void Scene::GenerateMesh()
{
	vector<GLfloat> front;
	vector<GLfloat> left;
	vector<GLfloat> top;
	vector<GLfloat> bottom;
	float h = wallHeight;
	float s = cellSize;

	float t = WALLTHICKNESS;
	int quads = 0;
	for (int j = 0; j < height*2; j++) {
		for (int i = 0; i < width; i++) {
			//front facing wall
			if (j % 2 == 0) {
				if (walls[i][j] != 0){
					front = {
					s*i*1.0f,		 0.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 1.0f, //bottom left
					s*(i + 1)*1.0f,  0.0f,		 s*(j / 2.0f)*1.0f,  1.0f, 1.0f, //bottom right
					s*(i + 1)*1.0f,  h*1.0f,	 s*(j / 2.0f)*1.0f,  1.0f, 0.0f, //top right
					s*(i + 1)*1.0f,  h*1.0f,	 s*(j / 2.0f)*1.0f,  1.0f, 0.0f, //top right
					s*i*1.0f,		 h*1.0f,	 s*(j / 2.0f)*1.0f,  0.0f, 0.0f, //top left
					s*i*1.0f,		 0.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 1.0f, //bottom left
				};
				quads++;
				vertices[walls[i][j] - 1].insert(vertices[walls[i][j] - 1].end(), front.begin(), front.end());
				Cwalls.push_back(Wall(i*s, (j*s / 2) - (t/2.0f), s, t));
				}
			}
			else {
				if (walls[i][j] != 0){
					left = {
					s*i*1.0f,	 0.0f,		 s*((j-1) / 2.0f)*1.0f,  0.0f, 1.0f, //bottom front
					s*(i)*1.0f,  0.0f,		 s*(((j - 1) / 2.0f) + 1.0f)*1.0f,  1.0f, 1.0f, //bottom back
					s*(i)*1.0f,  h*1.0f,	 s*((j - 1) / 2.0f)*1.0f,  0.0f, 0.0f, //top front
					s*(i)*1.0f,  h*1.0f,	 s*((j - 1) / 2.0f)*1.0f,  0.0f, 0.0f, //top front
					s*i*1.0f,	 h*1.0f,	 s*(((j - 1) / 2.0f) + 1.0f)*1.0f,  1.0f, 0.0f, //top back
					s*(i)*1.0f,  0.0f,		 s*(((j - 1) / 2.0f) + 1.0f)*1.0f,  1.0f, 1.0f, //bottom back
				};
				quads++;
				vertices[walls[i][j]-1].insert(vertices[walls[i][j] - 1].end(), left.begin(), left.end());
				Cwalls.push_back(Wall(i*s - (t / 2.0f), (j-1)*s / 2, t, s));
				}
			}
			if (j % 2 == 0) {
				top = {
					s*i*1.0f,		 h*1.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 0.0f, //bottom left
					s*(i + 1)*1.0f,  h*1.0f,		 s*(j / 2.0f)*1.0f,  1.0f, 0.0f, //bottom right
					s*(i + 1)*1.0f,  h*1.0f,	 s*((j / 2.0f) + 1)*1.0f,  1.0f, 1.0f, //top right
					s*(i + 1)*1.0f,  h*1.0f,	 s*((j / 2.0f) + 1)*1.0f,  1.0f, 1.0f, //top right
					s*i*1.0f,		 h*1.0f,	 s*((j / 2.0f) + 1)*1.0f,  0.0f, 1.0f, //top left
					s*i*1.0f,		 h*1.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 0.0f, //bottom left
				};
				quads++;
				vertices[numberOfTextures-1].insert(vertices[numberOfTextures - 1].end(), top.begin(), top.end());

				//floor
				bottom = {
					s*i*1.0f,		 0 * 1.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 0.0f, //bottom left
					s*(i + 1)*1.0f,  0 * 1.0f,		 s*(j / 2.0f)*1.0f,  1.0f, 0.0f, //bottom right
					s*(i + 1)*1.0f,  0 * 1.0f,	 s*((j / 2.0f) + 1)*1.0f,  1.0f, 1.0f, //top right
					s*(i + 1)*1.0f,  0 * 1.0f,	 s*((j / 2.0f) + 1)*1.0f,  1.0f, 1.0f, //top right
					s*i*1.0f,		 0 * 1.0f,	 s*((j / 2.0f) + 1)*1.0f,  0.0f, 1.0f, //top left
					s*i*1.0f,		 0 * 1.0f,		 s*(j / 2.0f)*1.0f,  0.0f, 0.0f, //bottom left
				};
				quads++;
				vertices[numberOfTextures - 1].insert(vertices[numberOfTextures - 1].end(), bottom.begin(), bottom.end());
			}
			// ceiling
		}
	}
	cout << "number of quads " << quads;
}

void Scene::GenerateTextures()
{

	unsigned char* image;
	int txwidth, txheight;
	glGenTextures(20, Textures);

	for (int i = 0; i < numberOfTextures; i++) {

		glGenTextures(1, &Textures[i]);
		glBindTexture(GL_TEXTURE_2D, Textures[i]);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps

		string _path = texturePaths[i].substr(0, texturePaths[i].find('.', 0)) + ".png";
		cout << _path.c_str();
		image = SOIL_load_image(_path.c_str(), &txwidth, &txheight, 0, SOIL_LOAD_RGB);

		texture2Ds[i].data = image;
		texture2Ds[i].width = txwidth;
		texture2Ds[i].height = txheight;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txwidth, txheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		//SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
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

bool Wall::isPointInside(glm::vec3 p)
{
	if (p.x<x + length&&p.x>x) {
		if (p.z<y + width&&p.z>y) {
			return true;
		}
	}
	return false;
}

Wall::Wall(int _x, int _y, float _l, float _w)
{
	x = _x;
	y = _y;
	length = _l;
	width = _w;
}

Wall::Wall()
{
	x = 0;
	y = 0;
	length = 0;
	width = 0;
}

Wall::~Wall()
{

}