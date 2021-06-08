#include "CubeComponent.h"

CubeComponent::CubeComponent(float sizeX, float sizeY, float sizeZ, float r, float g, float b, float a)
{
	glm::vec3 sx(sizeX);
	glm::vec3 sy(sizeY);
	glm::vec3 sz(sizeZ);
	glm::vec4 color(r, b, g, a);
	glm::vec3 p(0, 0, 0);

	//bottom
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, -sz.z), color, glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, -sz.z), color, glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, sz.z), color, glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, sz.z), color, glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, -sz.z), color, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, -sz.z), color, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, sz.z), color, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, sz.z), color, glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, -sz.z), color, glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, -sz.z), color, glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, sz.z), color, glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, sz.z), color, glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, -sz.z), color, glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, -sz.z), color, glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, sz.z), color, glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, sz.z), color, glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, -sz.z), color, glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, -sz.z), color, glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, -sz.z), color, glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, -sz.z), color, glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, -sy.y, sz.z), color, glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(-sx.x, sy.y, sz.z), color, glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, sy.y, sz.z), color, glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(p + glm::vec3(sx.x, -sy.y, sz.z), color, glm::vec2(0, 1), glm::vec3(0, 0, 1)));
}


CubeComponent::~CubeComponent()
{
}

void CubeComponent::draw()
{
	tigl::drawVertices(GL_QUADS, verts);
}
