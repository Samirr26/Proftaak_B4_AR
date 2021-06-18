#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

class CubeComponent : public DrawComponent
{
	std::vector<Vertex> verts;
public:
	CubeComponent(float sizeX, float sizeY, float sizeZ, float r, float g, float b, float a);
	~CubeComponent();


	virtual void draw() override;
};

