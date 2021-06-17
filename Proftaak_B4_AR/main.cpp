#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "EnemyComponent.h"
#include "ObjModel.h"
#include <time.h>
#include <iostream>
#include "FpsCam.h"
#include "Texture.h"
#include "FaceDetection.h"
#include "ThreadManagement.h"


#define _USE_MATH_DEFINES

#include <math.h>
#include <corecrt_math.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* camera;
Texture* texture;
FaceDetection faceDetection(0);

void init();
void update();
void draw();


void faceDetectionTask() {

	faceDetection.VideoDisplay();
}


int main(void)
{

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
std::list<GameObject*> objectArea;
float counterSin = 0;
double lastFrameTime = 0;
bool startingPoint = true;
GameObject* movingObject;

GameObject* player;
ObjModel* model;

void init()
{
	glEnable(GL_DEPTH_TEST);
	srand(static_cast <unsigned> (time(0)));

	/*GameObject* o1 = new GameObject();
	o1->position = glm::vec3(0, 0, 0);
	o1->addComponent(new ObjModel("models/car/honda_jazz.obj"));
	objects.push_back(o1);*/
	

	for (int x1 = 0; x1 < 4; x1 += 1)
	{
		// Cup gameobjects
		GameObject* o = new GameObject();
		o->position = glm::vec3(x1 + 2, 0, 0);
		o->rotation.y = x1 * .25f;
		o->scale = glm::vec3(0.03f, 0.03f, 0.03f);
		//o->addComponent(new CubeComponent(1.2, 1.2, 1.2, 1, 0, 0, 1));
		o->addComponent(new ObjModel("models/cup3/cup.obj"));
		//o->addComponent(new SpinComponent(5.0f));
		o->point = 90 * x1;

		objects.push_back(o);

	}

	//// Cube for faces
	//for (int x1 = 0; x1 < 4; x1 += 1)
	//{
	//	GameObject* o = new GameObject();
	//	o->position = glm::vec3(x1 + 2, 2.25, 0);
	//	o->rotation.y = x1 * .25f;
	//	//o->scale = glm::vec3(0.03f, 0.03f, 0.03f);
	//	o->addComponent(new CubeComponent(0.32, 0.32, 0.32, 1, 1, 1, 0));
	//	o->addComponent(new SpinComponent(5.0f));
	//	o->point = 90 * x1;

	//	objects.push_back(o);

	//}
	// Face
	GameObject* o2 = new GameObject();
	o2->position = glm::vec3(2, 2.25, 0);
	//o2->rotation.y = .25f;
	o2->rotation.x = -1.55f;
 	//o2->scale = glm::vec3(0.6f, 0.4f, 0.4f);
	o2->addComponent(new CubeComponent(0.31, 0.29, 0.29, 1, 1, 1, 1));
	//o2->addComponent(new SpinComponent(5.0f));
	o2->point = 90;

	objects.push_back(o2);

	// Steve
	GameObject* o = new GameObject();
	o->position = glm::vec3(2, 0, 0);
	//o->rotation.y = .25f;
	o->scale = glm::vec3(0.3f, 0.3f, 0.3f);
	o->addComponent(new ObjModel("models/steve/Steve.obj"));
	//o->addComponent(new SpinComponent(5.0f));
	o->point = 90;

	objects.push_back(o);

	

	// Carousel
	/*GameObject* o1 = new GameObject();
	o1->position = glm::vec3(2, 2, 2);
	o->rotation.y = .25f;
	o->scale = glm::vec3(0.003f, 0.003f, 0.003f);
	o1->addComponent(new ObjModel("models/btc/btc.obj"));
	o->addComponent(new SpinComponent(5.0f));
	o1->point = 90;

	objects.push_back(o1);*/

	

	camera = new FpsCam(window);
	//texture = new Texture("Resources/test1.jpg");

	//model = new ObjModel("models/car/honda_jazz.obj");



	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

GameObject* positionCircleX(GameObject* object, float degrees) {

	float radian = (degrees * M_PI) / 180;
	object->position.x = 10 * sin(radian);
	object->position.z = 10 * cos(radian);

	return object;
}

void circlePath(GameObject* object) {
	object->point += 0.5f;
	if (object->point >= 360)
	{
		object->point = 0;
	}
	float radian = (object->point * M_PI) / 180;
	object->position.x = 5 * sin(radian);
	object->position.z = 5 * cos(radian);

}

void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	camera->update(window);

	for (auto& o : objects) {
		/*for (auto& oArea : objectArea) {
			if (o->position.x > oArea->position.y || o->position.z > oArea->position.y)
			{
				if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > 0.5)
				{
					o->position.x += (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 50;
					o->position.z += (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 50;
				}
				else {
					o->position.x -= (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 50;
					o->position.z -= (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 50;
				}
			}
		}*/

		circlePath(o);



		o->update(deltaTime);
	}

	for (auto& oArea : objectArea) {
		oArea->update(deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		texture = new Texture("Resources/gras.jpg");
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		texture = new Texture("Resources/test.jpg");

}

void drawTerrain() {

	texture = new Texture("Resources/gras.jpg");
	tigl::shader->enableTexture(true);
	texture->bind();

	//temporary draw floor

	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PCTN(glm::vec3(-50, 0, -50), glm::vec4(1, 1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(-50, 0, 50), glm::vec4(1, 1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(50, 0, 50), glm::vec4(1, 1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	tigl::addVertex(Vertex::PCTN(glm::vec3(50, 0, -50), glm::vec4(1, 1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	tigl::end();

	tigl::shader->enableTexture(false);
}



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

	// TODO light misschien nog verbeteren
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


	
	//texture->bind();

	////temporary draw floor
	//tigl::begin(GL_QUADS);
	//tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, -50), glm::vec4(1, 0, 0, 1)));
	//tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, 50),  glm::vec4(0, 1, 0, 1)));
	//tigl::addVertex(Vertex::PC(glm::vec3(50, 0, 50), glm::vec4(0, 0, 1, 1)));
	//tigl::addVertex(Vertex::PC(glm::vec3(50, 0, -50), glm::vec4(0, 0, 1, 1)));
	//tigl::end();

	std::cout << "Status: ";
	std::cout << overwriting << std::endl;
	//if (!overwriting) {
	//
	//	if (maskOn) {
	//		try
	//		{
	//			texture = new Texture("Resources/Faces/detectedFaceWithMask.png");
	//		}
	//		catch (const std::exception&)
	//		{
	//			texture = new Texture("Resources/Faces/black.png");
	//		}
	//
	//	}
	//	else {
	//		try
	//		{
	//			texture = new Texture("Resources/Faces/detectedFace.png");
	//		}
	//		catch (const std::exception&)
	//		{
	//			texture = new Texture("Resources/Faces/black.png");
	//		}
	//	}
	//
	//}
	tigl::shader->enableTexture(true);
	texture->bind();
	

	

	for (auto& o : objects)
		o->draw();


	tigl::shader->enableTexture(false);


	for (auto& oArea : objectArea)
		oArea->draw();

	
}



