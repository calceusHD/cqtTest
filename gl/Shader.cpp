

#include "Shader.h"

Shader::Shader(std::string name, bool file, GLenum type)
{
	this->type = type;
	char *source;
	if (file)
		source = fileToBuf(name);
	else
		source = (char*)name.c_str();
        if (source == nullptr) {
            std::cout << "Shader: invalid source! name=" << name << std::endl;
        }
	this->id = glCreateShader(this->type);
	//std::cout << source << std::endl;
	glShaderSource(this->id, 1, (const char**)&source, 0);
	glCompileShader(this->id);

	free(source);
	int maxLength;
	glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);

	if (maxLength > 0)
	{
		char *infoLog = (char*)malloc(maxLength);
		glGetShaderInfoLog(this->id, maxLength, &maxLength, infoLog);
		std::cout<<infoLog<<std::endl;
		free(infoLog);
	}

}

Shader::~Shader() {
	// TODO Auto-generated destructor stub
}

char *Shader::fileToBuf(std::string name)
{
	std::fstream file(name.c_str(), std::fstream::in | std::fstream::binary );
	long len;
	char *buf;
	file.seekg(0, file.end);
	len = file.tellg();
	buf = (char*)malloc(len +1);
	file.seekg(0, file.beg);
	if (len < 1)
	{
		file.close();
		return NULL;
	}
	file.read(buf, len);
	file.close();
	buf[len] = 0;
	return buf;
}

