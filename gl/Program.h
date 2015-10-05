

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Shader.h"

class Program {
public:
	Program();
	virtual ~Program();

	void attach(Shader& shader);
	bool build();
	void use();
	unsigned int getUniformLocation(const char *name);

	bool hasVertex;
	bool hasTcs;
	bool hasTes;
	bool hasGeom;
	bool hasFrag;
	unsigned int id;

};

#endif /* PROGRAM_H_ */
