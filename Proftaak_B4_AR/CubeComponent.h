#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

class CubeComponent : public DrawComponent
{
	std::vector<Vertex> verts;
public:
	CubeComponent(float size, float r, float b, float g, float a);
	~CubeComponent();


	virtual void draw() override;
};

