#include <atlimage.h>
#include "FrameBuffer.h"

/* this function copies the color buffer to the given CImage */
void FrameBuffer :: copyColorBuffer(CImage *image) {
	int i, j;
	int pitch = image->GetPitch();
	GLubyte *ptr_dst = (GLubyte *)image->GetBits();
	GLubyte *ptr_src = getColorPtr(0, height - 1);
	/* note that CImage is upside down, so we start reading the framebuffer
	   from the top of the screen */

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			*(ptr_dst++) = *(ptr_src + 2);
			*(ptr_dst++) = *(ptr_src + 1);
			*(ptr_dst++) = *(ptr_src);

			ptr_src += 3;
		}

		ptr_src += -(width * 3) * 2;
		ptr_dst += pitch - (width * 3); 
	}
	return;
}

/* this function dumps the color framebuffer to the specified file.  I will
   use the CImage class to do this */
void FrameBuffer :: dumpColorBufferToFile(char *name) {
	CImage image;

	image.Create(width, height, 24);


	/* now copy color buffer to the CImage bit array */
	copyColorBuffer(&image);

	/* save the image to the file */
	image.Save(name);

	/* clean up the CImage */
	image.Destroy();

	return;
}


#include <windows.h>

/* this function dumps the data array to the screen using glDrawPixels() */
void FrameBuffer :: dumpToScreen(void) {

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);


	
	// Draw container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);



	return;
}

void FrameBuffer::Fill(GLubyte R, GLubyte G, GLubyte B)
{


	for (int i = 0; i < width*height; i++) {
		color_buffer[i*4] = R;
		color_buffer[i*4 + 1] = G;
		color_buffer[i*4 + 2] = B;
		color_buffer[i*4 + 3] = 255;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);


	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
}
