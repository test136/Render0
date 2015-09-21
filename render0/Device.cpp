#include "stdafx.h"
#include "Device.h"
#include <limits>

const double Device::minDouble = -std::numeric_limits<double>::max();

Device::Device(int width, int height, Color *frameMemory, double *zMemory)
	:width(width), height(height), transform(width, height),
	frameMemory(frameMemory), zMemory(zMemory), outFrame(true), outZ(true),
	foreColor(255, 255, 255, 1), backColor(0, 0, 0, 1)
{
	int memorySize = width * height;

	if (!frameMemory)
	{
		outFrame = false;
		this->frameMemory = new Color[memorySize];
	}
	if (!zMemory)
	{
		outZ = false;
		this->zMemory = new double[memorySize];
	}
	memset(this->frameMemory, 0, memorySize * sizeof(Color));
	memset(this->zMemory, 0, memorySize * sizeof(double));
	
	for (int i = 0; i != memorySize; ++i)
	{
		this->zMemory[i] = minDouble;
	}

	zBuffer = new double*[height];
	frameBuffer = new Color*[height];
	for (int i = 0; i != height; ++i)
	{
		zBuffer[i] = this->zMemory + i * width;
		frameBuffer[i] = this->frameMemory + i * width;
	}	
}

Device::~Device(void)
{
	delete[] frameBuffer;
	frameBuffer = NULL;
	delete[] zBuffer;
	zBuffer = NULL;
	if (!outFrame)
	{
		delete[] frameMemory;
	}
	if (!outZ)
	{
		delete[] zMemory;
	}
	frameBuffer = NULL;
	zMemory = NULL;
}

bool Device::createSuccess()
{
	return zBuffer != NULL && frameBuffer != NULL &&
		zMemory != NULL && frameMemory != NULL &&
		sizeof(zBuffer) == height * sizeof(double) &&
		sizeof(frameBuffer) == width * sizeof(Color);
}

bool Device::checkInScreen(int x, int y)
{
	return x >= 0 && x < height  && y >= 0 && y < width;
}

Device* Device::drawPixel(int x, int y, double z, const Color &color)
{
	if (checkInScreen(x, y) && z > zBuffer[x][y])
	{
		frameBuffer[x][y] = color;
		zBuffer[x][y] = z;
	}
	return this;
}

Device* Device::drawLine(int startx, int starty, int endx, int endy,
						 int z1, int z2,
						 Color color, Color color2)
{
	if (startx == endx && starty == endy)
	{
		drawPixel(startx, starty, max(z1, z2), color);
	}
	else if (startx == endx)
	{
		int fromx=startx, fromy=starty, tox=endx, toy=endy;
		if (starty > endy)
		{
			swap(fromx, tox);
			swap(fromy, toy);
			swap(z1, z2);
			swap(color, color2);
		}
		for (int y = fromy; y != toy; ++y)
		{
			double rate = static_cast<double>(y-fromy) / (toy-fromy);
			drawPixel(fromx, y, z1 + (z2-z1)*rate, color.interp(color2, rate));
		}
	}
	else if (starty == endy)
	{
		int fromx=startx, fromy=starty, tox=endx, toy=endy;
		if (startx > endx)
		{
			swap(fromx, tox);
			swap(fromy, toy);
			swap(z1, z2);
			swap(color, color2);
		}
		for (int x = fromx; x != tox; ++x)
		{
			double rate = static_cast<double>(x-fromx)/(tox-fromx);
			drawPixel(x, fromy, z1 + (z2-z1)*rate, color.interp(color2, rate));
		}
	}
	else
	{
		int dx = (startx < endx) ? endx - startx : startx - endx;
		int dy = (starty < endy) ? endy - starty : starty - endy;
		int fromx = startx, fromy = starty, tox = endx, toy = endy;
		if (dx >= dy)
		{
			if (fromx > tox)
			{
				swap(fromx, tox);
				swap(fromy, toy);
				swap(z1, z2);
				swap(color, color2);
			}
			int cumy = 0;
			for (int x = fromx, y = fromy; x != tox; ++x)
			{
				if (x >= height)
				{
					break;
				}
				double rate = static_cast<double>(x-fromx)/(tox-fromx);
				drawPixel(x, y, z1 + (z2-z1)*rate, color.interp(color2, rate));
				cumy += dy;
				if (cumy > dx)
				{
					cumy -= dx;
					y += fromy > toy ? -1 : 1;
					drawPixel(x, y, z1 + (z2-z1)*rate, color.interp(color2, rate));
				}
			}
			drawPixel(tox, toy, z2, color2);
		}
		else
		{
			if (fromy > toy)
			{
				swap(fromx, tox);
				swap(fromy, toy);
				swap(color, color2);
			}
			int cumx = 0;
			for (int y = fromy, x = fromx; y != toy; ++y)
			{
				if (y >= width)
				{
					break;
				}
				double rate = static_cast<double>(y-fromy)/(toy-fromy);
				drawPixel(x, y, z1 + (z2-z1)*rate, color.interp(color2, rate));
				cumx += dx;
				if (cumx > dy)
				{
					cumx -= dy;
					x += fromx > tox ? -1 : 1;
					drawPixel(x, y, z1 + (z2-z1)*rate, color.interp(color2, rate));
				}
			}
			drawPixel(tox, toy, z2, color2);
		}
	}
	return this;
}

Device *Device::drawVertex(Vertex &v)
{
	transform.updateTransform();
	Vector<double> res = transform.homogenize(v.getPos()).divideW();
	int x = static_cast<int>(res.getx());
	int y = static_cast<int>(res.gety());
	double z = res.getz();
	drawPixel(x, y, z, v.getColor());
	return this;
}

Device *Device::drawLine(Vertex &v1, Vertex &v2)
{
	transform.updateTransform();
	Vector<double> vec1 = transform.homogenize(v1.getPos()).divideW();
	Vector<double> vec2 = transform.homogenize(v2.getPos()).divideW();
	int x1 = static_cast<int>(vec1.getx());
	int y1 = static_cast<int>(vec1.gety());
	int x2 = static_cast<int>(vec2.getx());
	int y2 = static_cast<int>(vec2.gety());
	double z1 = vec1.getz();
	double z2 = vec2.getz();
	drawLine(x1, y1, x2, y2, z1, z2, v1.getColor(), v2.getColor());
	return this;
}

Device *Device::drawPane(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4)
{
	transform.updateTransform();
	Vector<double> vec1 = transform.homogenize(v1.getPos()).divideW();
	Vector<double> vec2 = transform.homogenize(v2.getPos()).divideW();
	Vector<double> vec3 = transform.homogenize(v3.getPos()).divideW();
	Vector<double> vec4 = transform.homogenize(v4.getPos()).divideW();

	int x1 = static_cast<int>(vec1.getx());
	int y1 = static_cast<int>(vec1.gety());
	double z1 = vec1.getz();

	int x2 = static_cast<int>(vec2.getx());
	int y2 = static_cast<int>(vec2.gety());
	double z2 = vec2.getz();

	int x3 = static_cast<int>(vec3.getx());
	int y3 = static_cast<int>(vec3.gety());
	double z3 = vec3.getz();

	int x4 = static_cast<int>(vec4.getx());
	int y4 = static_cast<int>(vec4.gety());
	double z4 = vec4.getz();

	drawLine(x1, y1, x2, y2, z1, z2, v1.getColor(), v2.getColor());
	drawLine(x2, y2, x3, y3, z2, z3, v2.getColor(), v3.getColor());
	drawLine(x3, y3, x4, y4, z3, z4, v3.getColor(), v4.getColor());
	drawLine(x4, y4, x1, y1, z4, z1, v4.getColor(), v1.getColor());

	return this;
}
