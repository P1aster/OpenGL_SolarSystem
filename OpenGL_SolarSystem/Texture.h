#pragma once
#include <GL/glut.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>

class Texture
{
private:
	GLuint textureID;
	char* FileName;
	std::vector<unsigned char> pixels;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	GLfloat mat_ambient_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_diffuse_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_specular_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_emmision_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat mat_shininess = 20.0f;


public:
	Texture();
	~Texture();
	void cleanup();
	bool LoadBMPImage(const char* FileName);
	Texture& setDefaultTextureParams();
	Texture& uploadTexture();
	Texture& setMaterialProps(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emmision[4], GLfloat shininess);
	Texture& setMaterialAmbient(GLfloat ambient[4]);
	Texture& setMaterialDiffuse(GLfloat diffuse[4]);
	Texture& setMaterialSpecular(GLfloat specular[4]);
	Texture& setMaterialEmmision(GLfloat emmision[4]);
	Texture& setMaterialShininess(GLfloat shininess);
	Texture& enableMaterial();

	void unbind() const;
	void bind() const;
	bool isLoaded() const;

	Texture& genTexture();

	GLuint getReturnID();

};

