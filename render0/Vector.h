#pragma once
#include <math.h>

typedef unsigned char Byte;


template <class T>
class Vector
{
public:
	Vector(const T x=0.0, const T y=0.0, const T z=0.0, const T w=1.0);
	Vector(const T *sourceArray);
	Vector(const Vector &v);
	~Vector();

	Vector operator+(const Vector &v) const;
	Vector operator-(const Vector &v) const;
	Vector operator*(const Vector &v) const;
	Vector &operator=(const Vector &v);

	Vector &operator+=(const Vector &v);
	Vector &operator-=(const Vector &v);
	Vector &operator*=(const Vector &v);
	T operator[](int index) const;
	double dotProduct(const Vector &v) const;
	Vector interp(const Vector &v2, const double r) const;

	void normalize();
	T getx() const;
	T gety() const;
	T getz() const;
	T getw() const;

	Vector divideW() const;

private:
	T x;
	T y;
	T z;
	T w;
};


template <class T>
Vector<T>::Vector(T x, T y, T z, T w)
	:x(x), y(y), z(z), w(w)
{
}

template <class T>
Vector<T>::Vector(const T *source)
	:x(source[0]), y(source[1]), z(source[2]), w(source[3])
{
}

template <class T>
Vector<T>::Vector(const Vector<T> &v)
	:x(v.x), y(v.y), z(v.z), w(v.w)
{
}

template <class T>
Vector<T>::~Vector()
{
}

template <class T>
Vector<T> Vector<T>::divideW() const
{
	return Vector<T>(x/w, y/w, z/w, (T)1.0);
}

template <class T>
void Vector<T>::normalize()
{
	double normalX = (double)x / (double)w;
	double normalY = (double)y / (double)w;
	double normalZ = (double)z / (double)w;

	double len = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
	x = static_cast<T>(normalX / len);
	y = static_cast<T>(normalY / len);
	z = static_cast<T>(normalZ / len);
	w = 1.0;
}

template <class T>
Vector<T> Vector<T>::interp(const Vector<T> &v, const double r) const
{
	return Vector<T>(x+(v.x-x)*r, y+(v.y-y)*r, z+(v.z-z)*r, 1.0);
}

template <class T>
Vector<T> Vector<T>::operator+(const Vector<T> &v) const
{
	return Vector<T>(x+v.x, y+v.y, z+v.z, v.w);
}

template <class T>
Vector<T> Vector<T>::operator-(const Vector<T> &v) const
{
	return Vector<T>(x-v.x, y-v.y, z-v.z, v.w);
}

template <class T>
Vector<T> Vector<T>::operator*(const Vector<T> &v) const
{
	double rate = 1.0 / (w * v.w);
	T newx = static_cast<T>((y*v.z-z*v.y) * rate);
	T newy = static_cast<T>((z*v.x - x*v.z) * rate);
	T newz = static_cast<T>((x*v.y-y*v.x) * rate);
	return Vector<T>(newx, newy, newz, 1.0);
}

template <class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template <class T>
T Vector<T>::operator[](int index) const
{
	return index == 0 ? x : index == 1 ? y : index == 2 ? z : index == 3 ? w : 0.0;
}

template <class T>
Vector<T> &Vector<T>::operator+=(const Vector<T> &v)
{
	double rate = w / v.w;
	x += static_cast<T>(v.x * rate);
	y += static_cast<T>(v.y * rate);
	z += static_cast<T>(v.z * rate);
	return *this;
}

template <class T>
Vector<T> &Vector<T>::operator-=(const Vector<T> &v)
{
	double rate = w / v.w;
	x -= static_cast<T>(v.x * rate);
	y -= static_cast<T>(v.y * rate);
	z -= static_cast<T>(v.z * rate);
	return *this;
}

template <class T>
Vector<T> &Vector<T>::operator*=(const Vector<T> &v)
{
	double rate = w / v.w;
	x = static_cast<T>((y*v.z-z*v.y) * rate);
	y = static_cast<T>((z*v.x - x*v.z) * rate);
	z = static_cast<T>((x*v.y-y*v.x) * rate);
	return *this;
}

template <class T>
double Vector<T>::dotProduct(const Vector<T> &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

template <class T>
T Vector<T>::getx() const
{
	return x;
}

template <class T>
T Vector<T>::gety() const
{
	return y;
}

template <class T>
T Vector<T>::getz() const
{
	return z;
}

template <class T>
T Vector<T>::getw() const
{
	return w;
}

