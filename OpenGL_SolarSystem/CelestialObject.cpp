#include "CelestialObject.h"


CelestialObject::CelestialObject(GLfloat radius, GLuint stacks, GLuint slices)
{
	this->radius = radius;
	this->stacks = stacks;
	this->slices = slices;
	// Generate vertices for the celestial object
	generateVerticies();
}

CelestialObject::~CelestialObject() {}


void CelestialObject::generateVerticies()
{
	// Generate vertices for the celestial object
	for (int stack = 0; stack <= stacks; ++stack) {
		GLfloat phi = std::numbers::pi / 2 - (stack * std::numbers::pi / stacks); // Latitude angle
		GLfloat y = radius * sin(phi);
		GLfloat scale = radius * cos(phi);

		for (int slice = 0; slice <= slices; ++slice) {
			GLfloat theta = slice * 2 * std::numbers::pi / slices; // Longitude angle

			GLfloat x = scale * cos(theta);
			GLfloat z = scale * sin(theta);

			GLfloat u = 1.0f - static_cast<GLfloat>(slice) / slices; // Texture coordinate u
			GLfloat v = 1.0f - static_cast<GLfloat>(stack) / stacks; // Inverted texture coordinate v

			// Calculate normal vector
			GLfloat length = sqrt(x * x + y * y + z * z); // Length of the normal vector
			GLfloat nx = x / length;
			GLfloat ny = y / length;
			GLfloat nz = z / length;

			Vertex vertex;

			vertex.position[0] = x;
			vertex.position[1] = y;
			vertex.position[2] = z;

			vertex.texCoord[0] = u;
			vertex.texCoord[1] = v;

			vertex.normal[0] = nx;
			vertex.normal[1] = ny;
			vertex.normal[2] = nz;

			vertices.push_back(vertex);
		}
	}
}


void CelestialObject::render()
{
	// Enable texturing before binding
	glEnable(GL_TEXTURE_2D);

	// Apply material properties
	texture.enableMaterial();

	// Bind texture
	texture.bind();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	// Draw the sphere using triangle strips
	glBegin(GL_TRIANGLE_STRIP);
	for (int stack = 0; stack < stacks; ++stack) {
		for (int slice = 0; slice <= slices; ++slice) {
			int first = stack * (slices + 1) + slice;
			int second = first + slices + 1;

			// First vertex (top triangle)
			glNormal3fv(vertices[first].normal);
			glTexCoord2fv(vertices[first].texCoord);
			glVertex3fv(vertices[first].position);

			// Second vertex (bottom triangle)
			glNormal3fv(vertices[second].normal);
			glTexCoord2fv(vertices[second].texCoord);
			glVertex3fv(vertices[second].position);
		}
	}
	glEnd();

	// Clean up GL state
	glDisable(GL_TEXTURE_2D);
}


void CelestialObject::draw(int i) {
	// Apply transformation
	applyTransform();

	// **Bind the texture before rendering**
	if (texture.isLoaded()) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		texture.bind();
	}

	// Render the celestial object
	render();

	// **Unbind the texture after rendering**
	if (texture.isLoaded()) {
		texture.unbind();
		glDisable(GL_TEXTURE_2D);
	}

	// Cleanup transformation
	cleanupTransform();
}


CelestialObject& CelestialObject::setRadius(GLfloat radius)
{
	this->radius = radius;
	generateVerticies();
	return *this;
}

CelestialObject& CelestialObject::setSlices(GLuint slices)
{
	this->slices = slices;
	generateVerticies();
	return *this;
}

CelestialObject& CelestialObject::setStacks(GLuint stacks)
{
	this->stacks = stacks;
	generateVerticies();
	return *this;
}