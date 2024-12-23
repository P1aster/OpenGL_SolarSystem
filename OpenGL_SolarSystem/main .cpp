#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include <math.h>
#include <tuple>
#include "Axes.h"
#include "Camera.h"
#include "MouseHandler.h"
#include <map>
#include "CelestialObject.h"

using namespace std;

GLfloat currentAngle = 0.0f;

CelestialObject sun(10.0f, 40, 40); // Define a sun object

GLfloat planetOrbitRadius = 15.0f;   // Radius of the planet's orbit
GLfloat planetRotationSpeed = 0.5f; // Speed of orbit rotation
GLfloat planetAngle = 0.0f;          // Current angle of the planet in its orbit

MouseHandler mouseHandler;

int mode = 1; 

// Camera settings
Camera camera;
GLdouble ZNear = 0.01f; // Near clipping plane
GLdouble ZFar = 1000.0f; // Far clipping plane
GLdouble pix2angle = 1.0;  // Pixel to angle ratio
GLdouble theta = 0.0;  // Rotate angle
GLdouble phi = 0.0;  // Rotate angle
GLdouble R = 10.0;  // Distance from the viewer


struct Planet {
    GLfloat radius;
    GLfloat orbitRadius;
    GLfloat orbitSpeed;
    GLfloat initialAngle;
    const char* texturePath;
};

// Example data for all planets
std::vector<Planet> planets = {
    {2.5f, 15.0f, 0.5f, 0.0f, "textures/bmp/2k_mercury.bmp"},
    {6.1f, 25.0f, 0.3f, 0.0f, "textures/bmp/2k_venus_surface.bmp"},
    {6.3f, 35.0f, 0.2f, 0.0f, "textures/bmp/2k_earth_daymap.bmp"},
    {3.4f, 45.0f, 0.1f, 0.0f, "textures/bmp/2k_mars.bmp"},
    {69.9f, 70.0f, 0.05f, 0.0f, "textures/bmp/2k_jupiter.bmp"},
    {58.2f, 120.0f, 0.02f, 0.0f, "textures/bmp/2k_saturn.bmp"},
    {25.4f, 170.0f, 0.015f, 0.0f, "textures/bmp/2k_uranus.bmp"},
    {24.6f, 200.0f, 0.01f, 0.0f, "textures/bmp/2k_neptune.bmp"}
};

std::vector<CelestialObject> planetObjects;


void initObjects() {
    for (const auto& obj : planets) {
        CelestialObject celestial(obj.radius, 40, 40);
        celestial.setObjectTexture(obj.texturePath);
        planetObjects.push_back(celestial);
    }
}

void initPlanets() {
    for (size_t i = 0; i < planets.size(); ++i) {
        const auto& p = planets[i];
        CelestialObject planet(p.radius, 30, 30);

        GLfloat ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat shininess = 50.0f;

        planet.setObjectMaterial(ambient, diffuse, specular, emission, shininess);
        planet.setObjectTexture(p.texturePath);
        planetObjects.push_back(planet);
    }
}

void simulateOrbit(CelestialObject& object, GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat orbitRadius, GLfloat rotationSpeed, GLfloat& currentAngle) {
    currentAngle += rotationSpeed;

    currentAngle = fmod(currentAngle, 360.0f);

    // Convert the angle to radians for trigonometric functions
    GLfloat angleInRadians = currentAngle * (std::numbers::pi / 180.0f);

    // Calculate the new position of the object on the orbit
    GLfloat x = centerX + orbitRadius * cos(angleInRadians);
    GLfloat z = centerZ + orbitRadius * sin(angleInRadians);

    // Update the position of the celestial object
    object.setPosition(x, centerY, z);

    // Rotate the object around its own axis
    object.setRotation(0.0f, currentAngle, 0.0f);

    auto rotation = object.getRotation();
    GLfloat selfRotationAngle = std::get<1>(rotation) + rotationSpeed;
    selfRotationAngle = fmod(selfRotationAngle, 360.0f);
    object.setRotation(rotation);
}

void simulateAllOrbits() {
    for (size_t i = 0; i < planetObjects.size(); ++i) {
        simulateOrbit(planetObjects[i], 0.0f, 0.0f, 0.0f, planets[i].orbitRadius, planets[i].orbitSpeed, planets[i].initialAngle);
    }
}

// Function to update viewport
void update_viewport(int horizontal, int vertical) {

    // Calculate the aspect ratio of the window
    GLfloat aspect = (GLfloat)horizontal / (GLfloat)vertical;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the perspective with the correct aspect ratio
    gluPerspective(70.0, aspect, ZNear, ZFar);

    // Set the viewport to cover the entire window
    glViewport(0, 0, horizontal, vertical);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render() {
    // Clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset the model/view matrix

    if (mouseHandler.isLeftMouseButtonPressed()) {
        theta = camera.getTheta() + mouseHandler.getDeltaX() * pix2angle;
        phi = camera.getPhi() + mouseHandler.getDeltaY() * pix2angle;
    }
    else if (mouseHandler.isRightMouseButtonPressed()) {
        R = Camera::clamp(camera.getR() + mouseHandler.getDeltaY() * pix2angle, ZNear, ZFar);
    }
    camera.updateCameraSpherePosition(theta, phi, R);

    camera.cameraLook(); // Apply camera transformations


    // Update light position to match sun's position
    auto sunPos = sun.getPosition();
    GLfloat light_position[] = {
        std::get<0>(sunPos),
        std::get<1>(sunPos),
        std::get<2>(sunPos),
        1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    sun.draw(0);
    int i = 1;
    simulateAllOrbits();

    for (auto& planet : planetObjects) {
        planet.draw(i);
        i++;
    }

    // Swap the front and back buffers
    glutSwapBuffers();
}

void keyboard_keys(unsigned char key, GLsizei x, GLsizei y)
{
    /*switch (key) {
    case 'u':
		mode = 1;
		break;
    default:
        return;
    }*/

    render();
}

void mouse_callback(int button, int state, GLsizei x, GLsizei y) {
    mouseHandler.mouseCallback(button, state, x, y);
}
void motion_callback(GLsizei x, GLsizei y) {
    mouseHandler.motionCallback(x, y);
    glutPostRedisplay();
}

// Initialize OpenGL settings and scene objects
void init() {

    glEnable(GL_TEXTURE_2D);

    // Set the clear color for the color buffer to a dark gray shade (RGBA: 0.1, 0.1, 0.1, 1.0)
    // This color will be used when clearing the color buffer with glClear
    glClearColor(0.0f, 0.0f, 0.03f, 1.0f);

    // Enable lighting calculations in OpenGL, allowing the scene to be lit by defined light sources
    glEnable(GL_LIGHTING);

    // Enable depth testing to ensure correct rendering of objects based on their distance from the camera
    // This prevents closer objects from being obscured by farther ones
    glEnable(GL_DEPTH_TEST);

    // Set the shading model to smooth (Gouraud shading), which interpolates vertex colors across polygons
    glShadeModel(GL_SMOOTH);


    glFrontFace(GL_CCW); // Counter-clockwise is front-facing
    glCullFace(GL_BACK); // Cull back-facing polygons


    glEnable(GL_LIGHT0);

    // Set up light properties
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };  
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };   
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


    sun.setObjectMaterial(
        new GLfloat[4]{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
        new GLfloat[4]{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
        new GLfloat[4]{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
        new GLfloat[4]{ 1.0f, 1.0f, 1.0f, 1.0f }, // emission
        0.0f // shininess
    );
    sun.setObjectTexture("textures/bmp/2k_sun.bmp");

	
    initPlanets();

}

void timer(int value) {
    render();
    glutTimerFunc(1000 / 60, timer, 0); // Schedule next timer event in 16.67 ms (approx. 60 FPS)
}


int main(int argc, char** argv) {

 

    //Initialize GLUT
    glutInit(&argc, argv);

    /*
        Initialize GLUT window mode. We It opens a window with the specified dimensions and properties
        such a double buffer mode, RGB color palette and enabled depth buffor.
    */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);

    // create the window 
    int window = glutCreateWindow("OpenGL Solar System");

    glewExperimental = GL_TRUE;  // Ensure GLEW initializes properly
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glutDisplayFunc(render);
    glutReshapeFunc(update_viewport);

    init();

    /*
        glutKeyboardFunc setup an callback function to be called when a standart key
        (like letters, numbers, or basic symbols) is pressed on the keyboard.
    */
    glutKeyboardFunc(keyboard_keys);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(motion_callback);

    // Set up the timer function to call the render function at 60 FPS
    glutTimerFunc(1000 / 60, timer, 0);

    // Enter the GLUT event processing loop
    glutMainLoop();
    return 0;
}