#include "./BGShader.h"
#include "../Rendering/ScreenQuad.h"

#include <sstream>
#include <fstream>
#include <iostream>

// Simple vertex shader to be shared between every fragment shader
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Pos;
    
    out vec4 v_Pos;
    
    void main() {
    
        gl_Position = vec4(a_Pos, 1);
    
    };
)";

static GLuint vertexShader;

void BGShader::Init(){
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);

    int success = 0;
	char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

using std::string;

BGShader::BGShader(const string& path) {

	//Read in background shader source
	using BufIt = std::istreambuf_iterator<char>;
	std::ifstream in(path);
	string fragmentSourceString = string(BufIt(in), BufIt());
	const char* fragmentSource = fragmentSourceString.c_str();

	int success = 0;
	char infoLog[512];

	//Compile Fragment Shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(fragmentShader);
}

void BGShader::use() {
	glUseProgram(ID);
}

void BGShader::setVec2(const char* name, float* value) {
	glUniform2fv(glGetUniformLocation(ID, name), 1, value);
}

void BGShader::setFloat(const char* name, GLfloat value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void BGShader::draw(float* res,float time) {
	use();
	glUniform2fv(glGetUniformLocation(ID, "iResolution"), 1, res);
	glUniform1f(glGetUniformLocation(ID, "iTime"), time);
	ScreenQuad::Draw();
}