#include "Object.h"


Object::Object()
{
	// Initialize all properties to default values
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };
}

void Object::setObjectTexture(const char* texturePath) {

    texture.enableMaterial();

    texture.cleanup();
    texture.genTexture();
	texture.bind();

    if (!texture.LoadBMPImage(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    texture.uploadTexture();
	texture.setDefaultTextureParams();

}

void Object::setObjectMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emmision[4], GLfloat shininess)
{
    texture.setMaterialProps(ambient, diffuse, specular, emmision, shininess);
    texture.enableMaterial();
}

// Transformation setters using tuples
void Object::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    position = std::make_tuple(x, y, z);
}

void Object::setPosition(const Object::Vector3& pos) {
    position = pos;
}

void Object::setRotation(GLfloat x, GLfloat y, GLfloat z) {
    rotation = std::make_tuple(x, y, z);
}

void Object::setRotation(const Object::Vector3& rot) {
    rotation = rot;
}

void Object::setScale(GLfloat x, GLfloat y, GLfloat z) {
    scale = std::make_tuple(x, y, z);
}

void Object::setScale(const Object::Vector3& sc) {
    scale = sc;
}

// Get methods for reading current state
Object::Vector3 Object::getPosition() const { return position; }
Object::Vector3 Object::getRotation() const { return rotation; }
Object::Vector3 Object::getScale() const { return scale; }

// Apply transformations before rendering
void Object::applyTransform() {
    glPushMatrix();

    // Apply transformations in order: translate, rotate, scale
    glTranslatef(x(position), y(position), z(position));

    // Apply rotations in Y-X-Z order
    glRotatef(y(rotation), 0.0f, 1.0f, 0.0f); // Y axis rotation first
    glRotatef(x(rotation), 1.0f, 0.0f, 0.0f); // Then X axis
    glRotatef(z(rotation), 0.0f, 0.0f, 1.0f); // Finally Z axis

    glScalef(x(scale), y(scale), z(scale));
}

// Clean up transformations after rendering
void Object::cleanupTransform() {
    glPopMatrix();
}

