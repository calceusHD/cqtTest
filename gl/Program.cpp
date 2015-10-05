/*
 * Program.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: ICH
 */

#include "Program.h"

Program::Program() {
	this->id = glCreateProgram();
	this->hasFrag = false;
	this->hasTcs = false;
	this->hasGeom = false;
	this->hasTes = false;
	this->hasVertex = false;

}

void Program::attach(Shader& shader) {
	switch (shader.type)
	{
	case GL_VERTEX_SHADER:
		this->hasVertex = true;
		break;
	case GL_TESS_CONTROL_SHADER:
		this->hasTcs = true;
		break;
	case GL_TESS_EVALUATION_SHADER:
		this->hasTes = true;
		break;
	case GL_GEOMETRY_SHADER:
		this->hasGeom = true;
		break;
	case GL_FRAGMENT_SHADER:
		this->hasFrag = true;
		break;
	}
	glAttachShader(this->id, shader.id);

}

bool Program::build()
{
	glLinkProgram(this->id);

	int isLinked, maxLen;

	glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLen);

	if (maxLen > 0)
	{
		char *infoLog = (char*)malloc(maxLen);

		glGetProgramInfoLog(this->id, maxLen, &maxLen, infoLog);

		std::cout << infoLog << std::endl;
		free(infoLog);
	}

	if (isLinked == GL_FALSE)
		return false;
	return true;
}

unsigned int Program::getUniformLocation(const char *name)
{
    return glGetUniformLocation(this->id, name);
}

void Program::use()
{
	glUseProgram(this->id);
}

Program::~Program() {
	// TODO Auto-generated destructor stub
}

