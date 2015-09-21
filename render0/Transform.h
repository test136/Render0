#pragma once

#include "Vector.h"
#include "Matrix.h"

class Transform
{
public:
	Transform(int width=800, int height=600);
	~Transform(void);

	void updateTransform();
	void init();
	void setPerspective(const double fovy, const double aspect,
		const double znear, const double zfar);
	void setLookAt(const Vector<double> eye, const Vector<double> lookAt, const Vector<double> up);
	Vector<double> apply(const Vector<double> &pos) const;
	Vector<double> homogenize(const Vector<double> &pos) const;

private:
	Matrix world;
	Matrix view;
	Matrix projection;
	Matrix transform;
	bool changeWorld;
	bool changeView;
	bool changeProjection;
	double width;
	double height;
};

