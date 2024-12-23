#pragma once
#include <GL/glut.h>
#include <tuple>
#include "Texture.h"

class Object
{

protected:

    // Using tuples to store related x,y,z coordinates
    using Vector3 = std::tuple<GLfloat, GLfloat, GLfloat>;

    // Transform properties
    Vector3 position;      // (x, y, z) position in 3D space
    Vector3 rotation;      // (x, y, z) rotation angles in degrees
    Vector3 scale;         // (x, y, z) scale factors

	Texture texture;


    // Helper functions to access tuple elements
    static GLfloat& x(Vector3& v) { return std::get<0>(v); }
    static GLfloat& y(Vector3& v) { return std::get<1>(v); }
    static GLfloat& z(Vector3& v) { return std::get<2>(v); }

    // Const versions for reading values
    static const GLfloat& x(const Vector3& v) { return std::get<0>(v); }
    static const GLfloat& y(const Vector3& v) { return std::get<1>(v); }
    static const GLfloat& z(const Vector3& v) { return std::get<2>(v); }


public:
	Object();

	virtual void setObjectTexture(const char* texturePath);
	virtual void setObjectMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emmision[4], GLfloat shininess);
	virtual void draw(int i) = 0;

	// Transformation setters using tuples
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setPosition(const Vector3& pos);

	void setRotation(GLfloat x, GLfloat y, GLfloat z);
	void setRotation(const Vector3& rot);

	void setScale(GLfloat x, GLfloat y, GLfloat z);
	void setScale(const Vector3& sc);


	// Get methods for reading current state
	Vector3 getPosition() const;
	Vector3 getRotation() const;
	Vector3 getScale() const;


	virtual void applyTransform();
	virtual void cleanupTransform();

};

