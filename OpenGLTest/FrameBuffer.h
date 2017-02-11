#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdlib.h>
#include <atlimage.h>
#include "globals.h"
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"

/* implements the framebuffer. you might want to modify it to add depth buffering and other capabilities */
class FrameBuffer {
	private:
		int width, height;
		const char *vertex_path = "blit.vert";
		const char *fragment_path = "blit.frag";

		/* all the buffers are defined from the lower left corner of the screen */
		GLubyte *color_buffer;		/* color buffer is unsigned bytes buffer size 3*w*h */



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
		/* constructor */
		GLuint VBO, VAO, EBO;
		GLuint screenTexture;
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
		void Fill(GLubyte R, GLubyte G, GLubyte B);
};



#endif		/* FRAMEBUFFER_H */


