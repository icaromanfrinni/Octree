#pragma once
#include "Linear_Algebra.h"

struct Camera
{
	Vector4Df eye, lookat, up;

	Camera()
	{
	}

	Camera(const Vector4Df &_position, const Vector4Df &_view, const Vector4Df &_up) : eye(_position), lookat(_view), up(_up)
	{
		up.normalize();
	}
};

Matrix4 ToWorld(const Vector4Df &position, const Vector4Df &lookat, const Vector4Df &vup) {
	const Vector4Df& k = (position - lookat).to_unitary();
	const Vector4Df& i = cross(vup, k).to_unitary();
	const Vector4Df& j = cross(k, i).to_unitary();

	Matrix4 m;
	m.row[0] = { i.x, j.x, k.x, position.x };
	m.row[1] = { i.y, j.y, k.y, position.y };
	m.row[2] = { i.z, j.z, k.z, position.z };
	m.row[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

	return m;
}
Matrix4 ToWorld(const Camera &cam) {
	return ToWorld(cam.eye, cam.lookat, cam.up);
}

Matrix4 ToCamera(const Vector4Df &position, const Vector4Df &lookat, const Vector4Df &vup) {
	const Vector4Df& k = (position - lookat).to_unitary();
	const Vector4Df& i = cross(vup, k).to_unitary();
	const Vector4Df& j = cross(k, i).to_unitary();;

	Matrix4 m;
	m.row[0] = { i.x, i.y, i.z, -dot(i, position) };
	m.row[1] = { j.x, j.y, j.z, -dot(j, position) };
	m.row[2] = { k.x, k.y, k.z, -dot(k, position) };
	m.row[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

	return m;
}
Matrix4 ToCamera(const Camera &cam) {
	return ToCamera(cam.eye, cam.lookat, cam.up);
}