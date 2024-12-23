#include "Texture.h"


Texture::Texture() {
	this->FileName = nullptr;
    this->ImWidth = 0;
    this->ImHeight = 0;
    this->ImComponents = 0;
    this->ImFormat = 0;
    this->textureID = 0;
}

Texture::~Texture() {
    cleanup();
}

void Texture::cleanup() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::isLoaded() const {
    return textureID != 0;
}

Texture& Texture::uploadTexture() {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImWidth, ImHeight, 0, GL_RGB,
        GL_UNSIGNED_BYTE, pixels.data());

	pixels.clear();

    return *this;
}

Texture& Texture::setDefaultTextureParams() {
    // Specify the minification filter for the texture.
    // GL_TEXTURE_MIN_FILTER determines how OpenGL filters the texture
    // when it is displayed at a smaller size than the original image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear filtering for smooth transitions.

    // Specify the magnification filter for the texture.
    // GL_TEXTURE_MAG_FILTER determines how OpenGL filters the texture
    // when it is displayed at a larger size than the original image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear filtering for smooth transitions.

    // Set the wrapping mode for the S (horizontal) texture coordinate.
    // GL_TEXTURE_WRAP_S defines the behavior for texture coordinates
    // outside the 0.0 to 1.0 range. GL_REPEAT causes the texture to repeat.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    // Set the wrapping mode for the T (vertical) texture coordinate.
    // GL_TEXTURE_WRAP_T defines the behavior for texture coordinates
    // outside the 0.0 to 1.0 range. GL_REPEAT causes the texture to repeat.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return *this;
}

Texture& Texture::genTexture() {
	glGenTextures(1, &textureID);
	return *this;
}

bool Texture::LoadBMPImage(const char* FileName) {

    // Open BMP file
    std::ifstream file(FileName, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open texture file: " << FileName << std::endl;
        return false;
    }

    // Read BMP header
    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    // Extract image dimensions
    ImWidth = *(int*)&header[18];
    ImHeight = *(int*)&header[22];


    // Calculate padding
    int padding = (4 - (ImWidth * 3) % 4) % 4;

    // Read pixel data
    std::vector<unsigned char> pixelsData(ImWidth * ImHeight * 3);

    // BMP stores pixels bottom-to-top, left-to-right, BGR format
    for (int y = ImHeight - 1; y >= 0; y--) {
        for (int x = 0; x < ImWidth; x++) {
            unsigned char color[3];
            file.read(reinterpret_cast<char*>(color), 3);

            // Convert BGR to RGB
            pixelsData[(y * ImWidth + x) * 3 + 0] = color[2]; // R
            pixelsData[(y * ImWidth + x) * 3 + 1] = color[1]; // G
            pixelsData[(y * ImWidth + x) * 3 + 2] = color[0]; // B
        }
        // Skip padding bytes
        file.ignore(padding);
    }

    pixels = pixelsData;

    return true;
}

// Function to retrieve the OpenGL texture ID
GLuint Texture::getReturnID() {
    return textureID;
}


Texture& Texture::setMaterialProps(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emmision[4], GLfloat shininess) {
    // Set the material properties
    memcpy(mat_ambient_color, ambient, 4 * sizeof(GLfloat));
    memcpy(mat_diffuse_color, diffuse, 4 * sizeof(GLfloat));
    memcpy(mat_specular_color, specular, 4 * sizeof(GLfloat));
    memcpy(mat_emmision_color, emmision, 4 * sizeof(GLfloat));
    mat_shininess = shininess;

    return *this;
}

Texture& Texture::setMaterialAmbient(GLfloat ambient[4]) {
    memcpy(mat_ambient_color, ambient, 4 * sizeof(GLfloat));
	return *this;
}

Texture& Texture::setMaterialDiffuse(GLfloat diffuse[4]) {
	memcpy(mat_diffuse_color, diffuse, 4 * sizeof(GLfloat));
	return *this;
}

Texture& Texture::setMaterialSpecular(GLfloat specular[4]) {
	memcpy(mat_specular_color, specular, 4 * sizeof(GLfloat));
	return *this;
}

Texture& Texture::setMaterialEmmision(GLfloat emmision[4]) {
	memcpy(mat_emmision_color, emmision, 4 * sizeof(GLfloat));
	return *this;
}

Texture& Texture::setMaterialShininess(GLfloat shininess) {
	mat_shininess = shininess;
	return *this;
}
Texture& Texture::enableMaterial() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_color);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision_color);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	return *this;
}

