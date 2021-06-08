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
double lastFrameTime = 0;
GameObject* movingObject;

GameObject* player;
ObjModel* model;

void init()
{
	glEnable(GL_DEPTH_TEST);
	srand(static_cast <unsigned> (time(0)));

	for (int x = -30; x < 30; x += 2) {

		/*GameObject	* o = new GameObject();
		o->position = glm::vec3(x, 0, 0);
		o->rotation.y = x * .25f;
		o->addComponent(new CubeComponent(2.0f));
		o->addComponent(new SpinComponent(2));
		objects.push_back(o);*/

		for (int  x1 = -8; x1 < 8; x1 += 4)
		{
			for (int  z1 = -8; z1 < 8; z1 += 4)
			{
				GameObject* o = new GameObject();
				o->position = glm::vec3(x1 + 2, 0, z1 - 2);
				o->rotation.y = x * .25f;
				o->addComponent(new CubeComponent(1.2f, 1, 0, 0, 1));
				o->addComponent(new SpinComponent(1));

				objects.push_back(o);
			}
			
		}

		for (int  x2 = -8; x2 < 8; x2 += 4)
		{
			GameObject* o = new GameObject();
			o->position = glm::vec3(x2 + 6, 1, x2 - 6);
			o->rotation.y = x * .25f;
			o->addComponent(new CubeComponent(2, 1, 0, 1, 1));
			o->addComponent(new SpinComponent(1));
		}
	
		
	}

	player = new GameObject();
	player->position = glm::vec3(0, 1, 5);
	player->addComponent(new CubeComponent(1, 1, 1, 1, 1));
	player->addComponent(new PlayerComponent());

	model = new ObjModel("models/car/honda_jazz.obj");
	
	objects.push_back(player);

	
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
}


void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	for (auto& o : objects) {
		if (o->position.x < 10 || o->position.z < 5)
		{
			if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > 0.5)
			{
				o->position.x += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				o->position.z += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			}
			else {
				o->position.x -= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				o->position.z -= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			}
			
			
		}

		o->update(deltaTime);
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
	//temporary draw floor
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, -50), glm::vec4(1, 0, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(-50, 0, 50), glm::vec4(0, 1, 0, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, 50), glm::vec4(0, 0, 1, 1)));
	tigl::addVertex(Vertex::PC(glm::vec3(50, 0, -50), glm::vec4(0, 0, 1, 1)));
	tigl::end();



	for (auto& o : objects)
		o->draw();
}



