#include <glad/glad.h>
#include <GLFW/glfw3.h>

void drawTriangle() {
	GLfloat positions[] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	// send vertex buffer to GPU
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// layout of buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// draw tri
	glUseProgram(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// cleanup
	glDeleteBuffers(1, &vbo);
}



int main()
{
	return 0;
}
