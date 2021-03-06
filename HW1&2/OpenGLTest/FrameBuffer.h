#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdlib.h>
#include <atlimage.h>
#include "globals.h"
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"
#include <string>
#include <iostream>
#include "Vec.h"

/* implements the framebuffer. you might want to modify it to add depth buffering and other capabilities */

struct Color {

public:
	GLubyte R;
	GLubyte G;
	GLubyte B;
	GLubyte A;
	Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		R = r; G = g; B = b; A = a;
	}
	Color() {
		R = 0;
		G = 0;
		B = 0;
		A = 0;
	}

	Color(GLubyte val) {
		R = val;
		G = val;
		B = val;
		A = 255;
	}

	void operator=(Color &rhs) {
		R = rhs.R;
		G = rhs.G;
		B = rhs.B;
		A = rhs.A;
	}

	static Color Random() {
		Color col;
		col.R = int(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 255.0f)));
		col.G = int(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 255.0f)));
		col.B = int(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 255.0f)));
		col.A = 255;
		return col;
	}

	static Color Random(int seed) {
		Color col;
		srand(seed);
		col.R = int(rand() % 256);
		col.G = int(rand() % 256);
		col.B = int(rand() % 256);
		col.A = 255;
		return col;
	}

};

class FrameBuffer {
	private:

		const char *vertex_path = "blit.vert";
		const char *fragment_path = "blit.frag";





									// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[32] = {
			// Positions          // Colors           // Texture Coords
			1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[6] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};
		
		

	public:
		/* all the buffers are defined from the lower left corner of the screen */
		GLubyte *color_buffer;		/* color buffer is unsigned bytes buffer size 3*w*h */
		/* constructor */
		GLuint VBO, VAO, EBO;
		GLuint screenTexture;
		int width, height;
		Shader shader = Shader(vertex_path, fragment_path);
			FrameBuffer(int _w, int _h) {
			width = _w;		height = _h;



			/* allocate the buffer */
			color_buffer = (GLubyte *)malloc(sizeof(GLubyte) * width * height * 4);


			glGenTextures(1, &screenTexture);
			glBindTexture(GL_TEXTURE_2D, screenTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);


			// Set our texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			
			//Preapare the quad

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// Color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			// TexCoord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

			glBindVertexArray(0); // Unbind VAO


		};

		/* destructor */
		~FrameBuffer() {
			if (color_buffer)
				free(color_buffer);
		};

		/* get color ptr */
		GLubyte *getColorPtr(int x, int y) {
			return (color_buffer + ((y * width + x)*3));
		};

		void copyColorBuffer(CImage *image);
		void dumpColorBufferToFile(char *name);
		void dumpToScreen();
		void DrawPoint(GLfloat x, GLfloat y, Color c);
		void DrawPixel(GLint x, GLint y, Color c);
		void DrawPoint(GLfloat x, GLfloat y, Color c, GLint radius);
		void DrawPointClipSpace(GLfloat x, GLfloat y, Color c, GLint radius);
		void DrawPointClipSpace(Vec3 point, Color c, GLint radius);
		void DrawTriangle(Vec3 v1, Vec3 v2, Vec3 v3, Color col);
		void Fill(GLubyte R, GLubyte G, GLubyte B);
};



#endif		/* FRAMEBUFFER_H */


