#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "EnemyComponent.h"
#include "ObjModel.h"
#include "FpsCam.h"
#include "Texture.h"
#include "FaceDetection.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <corecrt_math.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using tigl::Vertex;

GLFWwindow* window;
FpsCam* camera;
Texture* texture;
FaceDetection faceDetection(0);
Texture* originalTexture;

float speed = 0.5f;

void init();
void update();
void draw();


void faceDetectionTask() {
	faceDetection.VideoDisplay();
}


int main(void)
{
	//Start face detection thread
	std::thread faceDetectThread(faceDetectionTask);

	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

std::list<GameObject*> objects;
std::list<GameObject*> faceObjectList;
double lastFrameTime = 0;

GameObject* player;
ObjModel* model;

void init()
{
	//Set standard texture
	originalTexture = new Texture("Resources/Faces/black.png");
	glEnable(GL_DEPTH_TEST);
	srand(static_cast <unsigned> (time(0)));

	for (int x1 = 0; x1 < 4; x1 += 1)
	{
		// Cup gameobjects
		GameObject* o = new GameObject();
		o->position = glm::vec3(x1 + 2, 0, 0);
		o->rotation.y = x1 * .25f;
		o->scale = glm::vec3(0.03f, 0.03f, 0.03f);
		o->addComponent(new ObjModel("models/cup3/cup.obj"));
		o->addComponent(new SpinComponent(2.0f));
		o->point = 90 * x1;

		objects.push_back(o);
	}

	// Face
	GameObject* o2 = new GameObject();
	o2->position = glm::vec3(2, 2.25, 0);
	o2->rotation.x = -1.55f;
	o2->addComponent(new CubeComponent(0.31, 0.29, 0.29, 1, 1, 1, 1));
	o2->point = 90;

	faceObjectList.push_back(o2);

	for (int i = 0; i < 4; i++) {
		// Steve
		GameObject* o = new GameObject();
		o->position = glm::vec3(2, 0, 0);
		o->scale = glm::vec3(0.3f, 0.3f, 0.3f);
		o->addComponent(new ObjModel("models/steve/Steve.obj"));
		o->point = 90*i;

		objects.push_back(o);
	}

	// Set fps cam
	camera = new FpsCam(window);

	// Set close window key
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

// Algorithm to rotate the cups
void circlePath(GameObject* object) {
	object->point += speed;
	if (object->point >= 360)
	{
		object->point = 0;
	}
	float radian = (object->point * M_PI) / 180;
	object->position.x = 5 * sin(radian);
	object->position.z = 5 * cos(radian);

}

// Update method
void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	camera->update(window);

	//Update all objects
	for (auto& o : objects) {
		circlePath(o);
		o->update(deltaTime);
	}

	//Update face
	for (auto& oFace : faceObjectList) {
		circlePath(oFace);
		oFace->update(deltaTime);
	}

	// Set change face key
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		faceDetection.changeTexture = true;

}

// Draw terrain method
void drawTerrain() {
		// Set grass texture
		texture = new Texture("Resources/gras.jpg");
		tigl::shader->enableTexture(true);
		texture->bind();

		// Temporary draw floor
		tigl::begin(GL_QUADS);
		tigl::addVertex(Vertex::PCTN(glm::vec3(-50, 0, -50), glm::vec4(1, 1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3(-50, 0, 50), glm::vec4(1, 1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3(50, 0, 50), glm::vec4(1, 1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
		tigl::addVertex(Vertex::PCTN(glm::vec3(50, 0, -50), glm::vec4(1, 1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
		tigl::end();
		tigl::shader->enableTexture(false);
}


//Draw method
void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	// Set world lighting
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	// Set lighting properties
	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightPosition(0, glm::vec3(0, 10, 0));
	tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.15f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.8f, 0.8f, 0.8f));
	tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
	tigl::shader->setShinyness(32.0f);

	tigl::shader->enableColor(true);

	drawTerrain();

	//Check if mask is On or Off and if Enter key was pressed
	if (faceDetection.changeTexture) {
		if (faceDetection.maskOn) {
			texture = new Texture("Resources/Faces/detectedFaceWithMask.png");
			speed = 0.5f;
			originalTexture = texture;
		}
		else {
			texture = new Texture("Resources/Faces/detectedFace.png");
			speed = 0.0f;
			originalTexture = texture;
		}
	}
	else {
		// If no texture was selected go back to the original texture
		texture = originalTexture;
	}	

	tigl::shader->enableTexture(true);
	texture->bind();
	
	//Draw face
	for (auto& oFace : faceObjectList) {
		oFace->draw();
	}

	//Change texture to cup texture
	texture = new Texture("Resources/dblauw.jpg");
	
	//Draw other objects
	for (auto& o : objects)
		o->draw();

	tigl::shader->enableTexture(false);	
}



