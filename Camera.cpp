#include "Camera.h"

#include "AppWindow.h"
#include "GraphicsEngine.h"

#include "ConstantBuffer.h"

using namespace GDEngine;

Camera::Camera(std::string name) : AGameObject(name)
{
	CBCameraData cbData;
	constantBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBCameraData));

	RECT viewport = AppWindow::getInstance()->getClientWindowRect();
	this->width = (viewport.right - viewport.left);
	this->height = (viewport.bottom - viewport.top);
}

Camera::~Camera()
{
	delete constantBuffer;
	AGameObject::~AGameObject();
}

void Camera::onCreate()
{
	AGameObject::onCreate();
}

void Camera::update(float deltaTime)
{
	RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

	this->updateLocalMatrix();
	this->updateViewMatrix();
	this->updateProjectionMatrix();

	CBCameraData cbCameraData = {};
	cbCameraData.viewMatrix = this->viewMatrix;
	cbCameraData.projMatrix = this->projMatrix;

	renderSystem->getImmediateDeviceContext()->setConstantBuffer(constantBuffer, 1);
	constantBuffer->update(renderSystem->getImmediateDeviceContext(), &cbCameraData);
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam;
	worldCam.setIdentity();

	Matrix4x4 temp;

	temp.setIdentity();
	temp.setRotationX(this->m_localRotation.x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(this->m_localRotation.y);
	worldCam *= temp;

	temp.setIdentity();
	temp.setTranslation(this->m_localPosition);
	worldCam *= temp;

	worldCam.inverse();
	this->viewMatrix = worldCam;
}

void Camera::updateProjectionMatrix()
{
	Matrix4x4 proj;
	switch (type) {
		case 0:
			proj.setPerspectiveFovLH(
				fov, // fov
				(float)width / (float)height, // aspect
				0.1f, // near
				100.0f // far
			);
			break;

		case 1:
			proj.setOrthoLH(
				width / 100.0f,
				height / 100.0f,
				-100.0f, 100.0f
			);
			break;
		case 2:
			proj.setOrthoLH(
				width / 100.0f,
				height / 100.0f,
				-100.0f, 100.0f
			);
			break;
		default:
			proj.setPerspectiveFovLH(
				fov, // fov
				(float)width / (float)height, // aspect
				0.1f, // near
				100.0f // far
			);
			break;
	}
	this->projMatrix = proj;
}

void Camera::onDestroy()
{
	AGameObject::onDestroy();
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->viewMatrix;
}

Matrix4x4 Camera::getProjMatrix()
{
	return this->projMatrix;
}

void Camera::setProjectionType(int type)
{
	this->type = type;

}

void Camera::setWidth(float width)
{
	this->width = width;
}

void Camera::setHeight(float height)
{
	this->height = height;
}
