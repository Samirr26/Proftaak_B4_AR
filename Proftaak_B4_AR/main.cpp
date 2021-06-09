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

#define _USE_MATH_DEFINES

#include <math.h>
#include <corecrt_math.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;


void init();
void update();
void draw();

int main(void)
{
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

		model = new ObjModel("models/car/honda_jazz.obj");

	
		for (int  x1 = 0; x1 < 4; x1 += 1)
		{
				GameObject* o = new GameObject();
				o->position = glm::vec3(x1 + 2, 0, 0);
				o->rotation.y = x1 * .25f;
				o->addComponent(new CubeComponent(1.2, 1.2, 1.2, 1, 0, 0, 1));
				//o->addComponent(new ObjModel("models/teacup/cup.obj"));
				o->addComponent(new SpinComponent(5.0f));
				o->point = 90 * x1;

				objects.push_back(o);
			
			
		}




	
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}

void circlePath(GameObject* object) {
	object->point += 0.25f;
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

	for (auto& o : objects) {
		circlePath(o);
		
		o->update(deltaTime);
	}

	for (auto& oArea : objectArea) {
		oArea->update(deltaTime);
	}	
	

}



void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 1000.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0,10,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);

	//temporary draw floor
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, -50), glm::vec4(1, 0, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, 50), glm::vec4(0, 1, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, 50), glm::vec4(0, 0, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, -50), glm::vec4(0, 0, 1, 1)));
	tigl::end();

	

	//tigl::shader->enableTexture(true);

	//model->draw();

	//tigl::shader->enableTexture(false);



	for (auto& o : objects)
		o->draw();

	for (auto& oArea : objectArea)
		oArea->draw();
}



