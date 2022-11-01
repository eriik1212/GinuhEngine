#pragma once

#include "EO_Base.h"
#include "EditorWindow.h"

class GameObject;

class EO_Game :public EO_Base
{
public:
	EO_Game();
	~EO_Game();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void CleanUp();
};