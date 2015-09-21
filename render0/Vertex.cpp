#include "stdafx.h"
#include "Vertex.h"


Vertex::Vertex(const Vector<double> &pos, const Color &color)
	:pos(pos), color(color)
{
}


Vertex::~Vertex(void)
{
}

const Vector<double>& Vertex::getPos()
{
	return pos;
}

const Color& Vertex::getColor()
{
	return color;
}
