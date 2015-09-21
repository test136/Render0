#include "stdafx.h"
#include "Matrix.h"
#include <math.h>


Matrix::Matrix()
{
	memset(mArray, 0, 4*4*sizeof(double));
}

Matrix::Matrix(double **p)
{
	memcpy(mArray, p, 4*4*sizeof(double));
}

Matrix::Matrix(Matrix &m)
{
	memcpy(mArray, m.mArray, 4*4*sizeof(double));
}


Matrix::~Matrix(void)
{
}


Matrix Matrix::operator+(const Matrix &m) const
{
	double tempArray[4][4];
	const double (*nArray)[4] = m.mArray;
	for (int i=0; i != 4; ++i)
	{
		for(int k=0; k != 4; ++k)
		{
			tempArray[i][k] = mArray[i][k] + nArray[i][k];
		}
	}
	return Matrix((double **)tempArray);
}

Matrix Matrix::operator-(const Matrix &m) const
{
	double tempArray[4][4];
	const double (*nArray)[4] = m.mArray;
	for (int i=0; i != 4; ++i)
	{
		for (int k=0; k != 4; ++k)
		{
			tempArray[i][k] = mArray[i][k] - nArray[i][k];
		}
	}
	return Matrix((double **)tempArray);
}

Matrix Matrix::operator*(const Matrix &m) const
{
	double tempArray[4][4];
	const double (*nArray)[4] = m.mArray;
	for (int i = 0; i != 4; ++i)
	{
		for (int k = 0; k != 4; ++k)
		{
			double number = 0;
			for (int n = 0; n != 4; ++n) 
			{
				number += mArray[i][n]*nArray[n][k];
			}
			tempArray[i][k] = number;
		}
	}
	return Matrix((double **)tempArray);
}

Vector<double> Matrix::operator*(const Vector<double> &v) const
{
	double result[4];
	for (int i = 0; i != 4; ++i)
	{
		result[i] = 0.0;
		for (int k = 0; k != 4; ++k)
		{
			result[i] += mArray[i][k] * v[k];
		}
	}
	return Vector<double>(result);
}

Matrix &Matrix::operator^=(double rate)
{
	for (int i=0; i != 4; ++i)
	{
		for (int k=0; k != 4; ++k)
		{
			mArray[i][k] *= rate;
		}
	}
	return *this;
}

Matrix &Matrix::operator=(const Matrix &m)
{
	memcpy(mArray, m.mArray, 4*4*sizeof(double));
	return *this;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
	const double (*nArray)[4] = m.mArray;
	for (int i=0; i != 4; ++i)
	{
		for(int k=0; k != 4; ++k)
		{
			mArray[i][k] += nArray[i][k];
		}
	}
	return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
	const double (*nArray)[4] = m.mArray;
	for (int i=0; i != 4; ++i)
	{
		for(int k=0; k != 4; ++k)
		{
			mArray[i][k] -= nArray[i][k];
		}
	}
	return *this;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
	const double (*nArray)[4] = m.mArray;
	for (int i=0; i != 4; ++i)
	{
		for (int k=0; k != 4; ++k)
		{
			double number = 0;
			for (int n=0; n != 4; ++n) 
			{
				number += mArray[i][n]*nArray[n][k];
			}
			mArray[i][k] = number;
		}
	}
	return *this;
}

Matrix &Matrix::operator/=(const Matrix &m)
{
	return *this;
}

double *Matrix::operator[](int index)
{
	return mArray[index];
}


void Matrix::setIdentity()
{
	for (int i=0; i != 4; ++i)
	{
		for (int k=0; k != 4; ++k)
		{
			mArray[i][k] = i == k ? 1.0 : 0.0;
		}
	}
}

void Matrix::setZero()
{
	for (int i=0; i != 4; ++i)
	{
		for(int k = 0; k != 4; ++k)
		{
			mArray[i][k] = 0.0;
		}
	}
}

Matrix Matrix::translate(double x, double y, double z)
{
	Matrix transformMatrix;
	transformMatrix.setIdentity();
	transformMatrix[0][3] = x;
	transformMatrix[1][3] = y;
	transformMatrix[2][3] = z;
	transformMatrix[3][3] = 1.0;
	return transformMatrix;
}

Matrix Matrix::scale(double x, double y, double z)
{
	Matrix transformMatrix;
	transformMatrix[0][0] = x;
	transformMatrix[1][1] = y;
	transformMatrix[2][2] = z;
	transformMatrix[3][3] = 1.0;
	return transformMatrix;
}

Matrix Matrix::rotate(double angle, double x, double y, double z)
{
	Matrix transformMatrix;
	double cosv = cos(angle);
	double _cosv = 1 - cosv;
	double sinv = sin(angle);
	double _sinv = 1 - sinv;
	transformMatrix[0][0] = cosv + _cosv * x * x;
	transformMatrix[0][1] = _cosv * x * y - sinv * z;
	transformMatrix[0][2] = _cosv * x * z + sinv * y;
	transformMatrix[0][3] = 0.0;

	transformMatrix[1][0] = _cosv * y * x + sinv * z;
	transformMatrix[1][1] = cosv + _cosv * y * y;
	transformMatrix[1][2] = _cosv * y * z - sinv * x;
	transformMatrix[1][3] = 0.0;

	transformMatrix[2][0] = _cosv * z * x - sinv * y; 
	transformMatrix[2][1] = _cosv * z * y + sinv * x;
	transformMatrix[2][2] = cosv + _cosv * z * z;
	transformMatrix[2][3] = 0.0;

	transformMatrix[3][0] = 0.0;
	transformMatrix[3][1] = 0.0;
	transformMatrix[3][2] = 0.0;
	transformMatrix[3][3] = 1.0;
	return transformMatrix;
}
