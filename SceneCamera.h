#pragma once

#include "Camera.h"

#include "InputListener.h"

namespace GDEngine
{
	class SceneCamera : public Camera, public InputListener
	{
	private:
		float speed = 1.0f;

		bool isControllable = false;

	public:
		SceneCamera(std::string name);
		~SceneCamera();

	public:
		void update(float deltaTime) override;
		void setSpeed(float speed);
		void setControllable(bool controllable);

		virtual void onKeyDown(int key);
		virtual void onKeyUp(int key);

		virtual void onMouseMove(const Vector2D& deltaMousePosition);

		virtual void onLeftMouseDown(const Vector2D& mousePosition);
		virtual void onLeftMouseUp(const Vector2D& mousePosition);

		virtual void onRightMouseDown(const Vector2D& mousePosition);
		virtual void onRightMouseUp(const Vector2D& mousePosition);

	};
}