#pragma once
#include "Vector.h"
class Matrix
{
public:
	Matrix();
	Matrix(double **p);
	Matrix(Matrix &m);
	~Matrix();

	Matrix operator+(const Matrix &m) const;
	Matrix operator-(const Matrix &m) const;
	Matrix operator*(const Matrix &m) const;
	Matrix operator/(const Matrix &m) const;
	Vector<double> operator*(const Vector<double> &v) const;
	Matrix &operator^=(const double rate);

	Matrix &operator=(const Matrix &m);
	Matrix &operator+=(const Matrix &m);
	Matrix &operator-=(const Matrix &m);
	Matrix &operator*=(const Matrix &m);
	Matrix &operator/=(const Matrix &m);
	double *operator[](int index);

	void setIdentity();
	void setZero();

	static Matrix translate(double x, double y, double z);
	static Matrix scale(double x, double y, double z);
	static Matrix rotate(double angle, double x, double y, double z);

private:
	double mArray[4][4];
};

