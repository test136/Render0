#pragma once

#include "Vector.h"

typedef Vector<Byte> Color;
class Vertex
{
public:
	Vertex(const Vector<double> &pos, const Color &color);
	~Vertex(void);

	const Vector<double>& getPos();
	const Color& getColor();
private:
	Vector<double> pos;
	Color color;
};

