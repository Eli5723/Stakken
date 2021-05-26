#include "ScreenQuad.h"

#include <GL/gl3w.h>

namespace ScreenQuad {

	const float vertices[]{
		-1,-1,1, //Top left
		1,-1,1,  // Top Right
		1,1,1,   // Bottom Left
		-1,1,1,  //Top left
		1,1,1,   // Top Right
		-1,-1,1, // Bottom right
	};

	GLuint vertexArray;
	GLuint vertexBuffer;

	void Init(){
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
	}

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		glBindVertexArray(vertexArray);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}