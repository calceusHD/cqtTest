/*
 * Shader.h
 *
 *  Created on: Mar 15, 2013
 *      Author: ICH
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>

#include "GL/glew.h"


class Shader {
public:
	Shader(std::string name, bool file, GLenum type);
	virtual ~Shader();

	char *fileToBuf(std::string name);

	GLenum type;
	unsigned int id;
};

#endif /* SHADER_H_ */
