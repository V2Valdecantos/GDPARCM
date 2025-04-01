#pragma once

#include "Window.h"

#include "SwapChain.h"
#include "ConstantBuffer.h"
#include "RasterizerState.h"

#include "InputListener.h"

using namespace GDEngine;

enum EFillMode : int;

class AppWindow : public Window, public InputListener
{
private:
	SwapChain* swapChain;

	ConstantBuffer* constantBuffer;

	RasterizerState* solidState;
	RasterizerState* wireframeState;

public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Vector2D& deltaMousePosition) override;

	virtual void onLeftMouseDown(const Vector2D& mousePosition) override;
	virtual void onLeftMouseUp(const Vector2D& mousePosition) override;

	virtual void onRightMouseDown(const Vector2D& mousePosition) override;
	virtual void onRightMouseUp(const Vector2D& mousePosition) override;

	void initializeEngine();

public:
	void update();
	void draw(int width, int height, EFillMode fillMode);
	SwapChain* getSwapChain();
private:
	static AppWindow* P_SHARED_INSTANCE;

private:
	AppWindow();
	~AppWindow();
	AppWindow(AppWindow const&);
	AppWindow& operator = (AppWindow const&);

public:
	static AppWindow* getInstance();
	static void initialize();
	static void destroy();
};