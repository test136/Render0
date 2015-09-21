#pragma once

#include "Transform.h"
#include "Vertex.h"
#undef max
class Device
{

public:
	Device(int width=800, int height=600,
		Color *frameMemory=NULL, double *zMemory=NULL);
	~Device(void);

	bool createSuccess();
	bool checkInScreen(int x, int y);
	Device* drawPixel(int x, int y, double z, const Color &color);
	Device* drawLine(int startx, int starty, int endx, int endy,
		int startz, int endz,
		Color color, Color color2);

	Device *drawVertex(Vertex &v);
	Device *drawPane(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4);
	Device *drawLine(Vertex &v1, Vertex&v2);

private:
	inline void swap(int &x, int &y)
	{
		int temp = x;
		x = y;
		y = temp;
	}
	inline void swap(Color &x, Color &y)
	{
		Color temp = x;
		x = y;
		y = temp;
	}
	inline int max(int x, int y)
	{
		return x > y ? x : y;
	}

	inline double max(double x, double y)
	{
		return x > y ? x : y;
	}


public:
	Transform transform;
	static const double minDouble;

private:
	bool outFrame;
	Color *frameMemory;
	bool outZ;
	double *zMemory;
	int width;
	int height;
	double **zBuffer;
	Color **frameBuffer;
	Color foreColor;
	Color backColor;
};

