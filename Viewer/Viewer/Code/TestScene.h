#pragma once

#include "Scene.h"
#include "ShaderFactory.hpp"

#include "SimpleSoundPlayer.h"
#include "SimpleText2D.h"

#include "System.h"

class TestScene : public Core::Scene
{
private:
	System*						_mipsEmulator;

private:
	UI::SimpleImage2D*			_background;


public:
	TestScene(void);
	~TestScene(void);

public:
	virtual void OnInitialize();
	virtual void OnRenderPreview();
	virtual void OnInput(const Device::Win32::Mouse& mouse, const  Device::Win32::Keyboard& keyboard);
	virtual void OnUpdate(float dt);
	virtual void OnRenderPost();
	virtual void OnDestroy();
};
