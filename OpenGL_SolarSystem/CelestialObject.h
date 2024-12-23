#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include "Vertex.h"
#include "Object.h"
#include <numbers>
#include <vector>
class CelestialObject : public Object
{
private:
	GLfloat radius = 10.0f;
	GLuint slices = 10;
	GLuint stacks = 10;
	GLfloat rotationAngle;
	std::vector<Vertex> vertices;
	void generateVerticies();
	void render();

public:
	CelestialObject(GLfloat radius, GLuint stacks, GLuint slices);
	~CelestialObject();
	void draw(int i) override;
	//void setObjectTexture(const char* textureFilePath);
	//void setObjectMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emmision[4], GLfloat shininess);
	//CelestialObject& enableObjectMaterial();
	CelestialObject& setRadius(GLfloat radius);
	CelestialObject& setSlices(GLuint slices);
	CelestialObject& setStacks(GLuint stacks);
};

