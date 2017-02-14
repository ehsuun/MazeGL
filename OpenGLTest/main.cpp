#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"
#include "Shader.h"
#include <SOIL.h>
#include "gltext.h"
#include "FrameBuffer.h"
#include "globals.h"
#include "Camera.h"
#include "Vec.h"


using namespace std;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void checkForCollision();


// Window dimensions


const char *vertex_path = "shader.vert";
const char *fragment_path = "shader.frag";

glm::vec3 cameraPos = glm::vec3(2.0f, 5.0f, 2.0f);
glm::vec3 lastCameraPos = glm::vec3(0.0f, 5.0f, -2.0f);

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


Vec3 cameraPos_s;
Vec3 cameraFront_s;
Vec3 cameraUp_s;
Vec3 lastCameraPos_s;



float wallHeight = 10.0f;

bool keys[1024];

GLfloat deltaTime = 0.0f;	
GLfloat lastFrameTime = 0.0f;  

GLfloat lastX = 0, lastY = 0;

GLfloat yaw = -90, pitch = 0;

GLfloat timer = 30;

bool firstMouse = true;

bool ifSoftware = false;

vector <Wall> *walls;


std::string readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void MoveAround();

// The MAIN function, from here we start the application and run the game loop
int main()
{

	 cameraPos_s = Vec3(cameraPos);
	 cameraFront_s = Vec3(cameraFront);
	 cameraUp_s = Vec3(0,1,0);
	 lastCameraPos_s = Vec3(lastCameraPos);

	std::cout << "Starting GLFW context, OpenGL 3.3, MazeGL" << std::endl;
	//

	Scene s = Scene(5,5);
	s.LoadLevel(readFile("maze1.txt"));
	s.GenerateMesh();

	wallHeight = s.wallHeight;

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "MyOpenGLTest", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader myShader(vertex_path, fragment_path);
	vector <Shader> shaders;

	for (int i = 0; i < s.numberOfTextures; i++) {
		shaders.push_back(Shader(vertex_path, fragment_path));
		//shaders[i].Use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, s.Textures[i]);
		glUniform1i(glGetUniformLocation(shaders[i].Program, "ourTexture"), 10);

	}

	s.GenerateTextures();

	walls = &s.Cwalls;

	GLuint VBOs[20], VAOs[20];

	GLuint EBOs[20];

	for (int i = 0; i < s.numberOfTextures; i++) {
	glGenBuffers(1, &EBOs[i]);

	glGenVertexArrays(1, &VAOs[i]);
	glGenBuffers(1, &VBOs[i]);

	// 1. Bind Vertex Array Object
	glBindVertexArray(VAOs[i]);
	// 2. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*s.vertices[i].size(), &s.vertices[i][0], GL_STATIC_DRAW);
	// 3. Copy our index array in a element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	if (i == 1) {
		cout << "true";
	}
	// 4. Unbind VAO (NOT the EBO)
	glBindVertexArray(0);
	}

	glm::mat4 model;
	model = glm::rotate(model, (float)glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view;
	
	Matrix44 modelSoftware;
	Matrix44 viewSoftware;

	// Note that we're translating the scene in the reverse direction of where we want to move
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
	Vec3 add = (cameraPos_s + cameraFront_s);
	//viewSoftware = Transform::LookatMatrixFW(cameraPos_s, add, cameraUp_s);
	viewSoftware = Transform::lookAt(cameraPos_s, add, cameraUp_s);
	//viewSoftware = view;

	for (int k = 0; k < 4; k++) {
		cout << endl;
		for (int l = 0; l < 4; l++) {
			cout << view[l][k] << "	";
		}
	}

	cout << "_______________________" << endl;

	for (int k = 0; k < 4; k++) {
		cout << endl;
		for (int l = 0; l < 4; l++) {
			cout << viewSoftware.M[l][k] << "	";
		}
	}



	glm::mat4 projection;
	projection = glm::perspective((float)glm::radians(45.0f), (float)window_width / (float)window_height, 0.1f, 500.0f);

	Camera cam = Camera(45,1,0.1f,500.0f);

	cam.gluPerspective();
	cam.glFrustum();

	Vec3 point = Vec3(0, 0, 0);




	glEnable(GL_DEPTH_TEST);


	FrameBuffer buffer = FrameBuffer(window_width, window_height);
	
	buffer.Fill(100, 100, 0);



	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		deltaTime = glfwGetTime() - lastFrameTime;
		lastFrameTime = glfwGetTime();
		timer -= deltaTime;




		cam.gluPerspective();
		cam.glFrustum();


		glfwPollEvents();
		MoveAround();
		//model = glm::rotate(model, 0.01f, glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		Vec3 add = (cameraPos_s + cameraFront_s);
		viewSoftware = Transform::lookAt(cameraPos_s, add, cameraUp_s);

		

		// Rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		//UI
		// Initialize glText
		gltInit();

		// Creating text
		GLTtext *text = gltCreateText();

		std::stringstream ss;
		ss<< "Remaining Time: " << std::fixed << std::setprecision(2) << timer;
		std::string mystring = ss.str();
		gltSetText(text, mystring.c_str());

		if (timer < 0)
		{
			gltSetText(text, "Game Over");
			gltColor(1.0f, 0.0f, 0.0f, 1.0f);
			gltDrawText2D(text, 750.0, 100.0f, 4.0);
		}
		else {		
			// Drawing text
			gltColor(1.0f, 1.0f, 1.0f, 1.0f);
			gltDrawText2D(text, 800.0, 100.0f, 2.0);
		}



		// Deleting text
		gltDeleteText(text);

		// Destroy glText
		gltTerminate();






		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(myShader.Program, "ourColor");

		for (int i = 0; i < s.numberOfTextures; i++) {

			shaders[i].Use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, s.Textures[i]);
			glUniform1i(glGetUniformLocation(shaders[i].Program, "ourTexture"), 0);

			//Sending Matrix Uniforms to shader
			GLint modelLoc = glGetUniformLocation(shaders[i].Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLint viewLoc = glGetUniformLocation(shaders[i].Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projectionLoc = glGetUniformLocation(shaders[i].Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


			glBindVertexArray(VAOs[i]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, s.vertices[i].size() / 5);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		if (ifSoftware) {
			buffer.Fill(0, 0, 0);
			// do software rendering here




			for (int j = 0; j < 20; j++) {
				for (int i = 0; i < s.vertices[j].size()/5; i++) {
					Vec3 point =
						Vec3(s.vertices[j].at(i * 5),
							s.vertices[j].at((i * 5)+1),
							s.vertices[j].at((i * 5)+2));
					Vec3 projected;
					Transform::multPointMatrix(point, projected, viewSoftware);
					Transform::multPointMatrix(projected, projected, cam.Mproj);
					buffer.DrawPointClipSpace(projected.x, projected.y, Color(255, 255, 255, 255), 5);
				}
			}


			buffer.DrawPoint(0.98, 0.98, Color(255, 255, 255, 255),50);
			buffer.dumpToScreen();
		}
		


		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAOs[0]);
	glDeleteBuffers(1, &VBOs[0]);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		ifSoftware = !ifSoftware;

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	cameraFront_s = Vec3(cameraFront);

}

void MoveAround()
{
	// Camera controls
	GLfloat cameraSpeed = 15.0f*deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	cameraPos_s = Vec3(cameraPos);
	checkForCollision();

	lastCameraPos = cameraPos;
}

void checkForCollision() {
	if (cameraPos.y < 0.2) {
		cameraPos = glm::vec3(cameraPos.x, 0.2, cameraPos.z);
		cameraPos_s = Vec3(cameraPos);
	}

	if (cameraPos.y > wallHeight -0.2f) {
		cameraPos = glm::vec3(cameraPos.x, wallHeight - 0.2f, cameraPos.z);
		cameraPos_s = Vec3(cameraPos);
	}


	for (int i = 0; i < walls->size(); i++) {

		if( walls->at(i).isPointInside(cameraPos)) {
			cameraPos = lastCameraPos;
			break;
		}
	}




}
