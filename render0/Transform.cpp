#include "stdafx.h"
#include <math.h>
#include "Transform.h"


Transform::Transform(int width, int height)
	:changeWorld(false), changeView(false), changeProjection(false),
	width(width), height(height)
{
	double aspect = width / height;
	init();

}


Transform::~Transform(void)
{
}


void Transform::init()
{
	world.setIdentity();
	view.setIdentity();
	projection.setIdentity();
}

void Transform::setPerspective(const double fovy, const double aspect,
							   const double znear, const double zfar)
{
	changeProjection = true;
	double fax = 1.0f / tan(0.5 * fovy);
	projection.setZero();
	projection[0][0] = fax / aspect;
	projection[1][1] = fax;
	projection[2][2] = zfar / (zfar - znear);
	projection[3][2] = 1.0;
	projection[2][3] = -znear * zfar / (zfar - znear);
}

void Transform::setLookAt(const Vector<double> eye,
						  const Vector<double> lookAt,
						  const Vector<double> up)
{
	changeView = true;
	Vector<double> diff = lookAt - eye;
	diff.normalize();
	Vector<double> a = up * diff;
	a.normalize();
	Vector<double> b = diff * a;

	view[0][0] = a.getx();
	view[0][1] = a.gety();
	view[0][2] = a.getz();
	view[0][3] = -a.dotProduct(eye);

	view[1][0] = b.getx();
	view[1][1] = b.gety();
	view[1][2] = b.getz();
	view[1][3] = -b.dotProduct(eye);

	view[2][0] = diff.getx();
	view[2][1] = diff.gety();
	view[2][2] = diff.getz();
	view[2][3] = -diff.dotProduct(eye);

	view[3][0] = 0.0;
	view[3][1] = 0.0;
	view[3][2] = 0.0;
	view[3][3] = 1.0;
}

void Transform::updateTransform()
{
	if (changeWorld || changeView || changeProjection)
	{
		transform =  projection * view * world;
		changeWorld = false;
		changeView = false;
		changeProjection = false;
	}
}

Vector<double> Transform::apply(const Vector<double> &pos) const
{
	return transform * pos;
}

Vector<double> Transform::homogenize(const Vector<double> &pos) const
{
	Vector<double> v = (transform * pos).divideW();
	return Vector<double>(
		(v.getx() + 1.0) * width * 0.5,
		(1.0 - v.gety()) * height * 0.5,
		v.getz(), 
		1.0);
}
