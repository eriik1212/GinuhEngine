#pragma once

#include "EO_Base.h"
#include "EditorWindow.h"

class GameObject;

enum class EW_TYPE
{
	HIERARCHY,
	ABOUT,
	GAME,
	SCENE,
	PERFORMANCE,
	INSPECTOR,
	CONSOLE,
	PROJECT,
	NUM_EW_TYPE, // number of EditorWindows types
};

class EO_Editor :public EO_Base
{
public:
	EO_Editor();
	~EO_Editor();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void CleanUp();

	void SetSelectedGameObject(GameObject* go)
	{
		selected_go = go;
	}
	GameObject* GetSelectedGameObject()
	{
		return selected_go;
	}

private:

	EditorWindow* editor_windows[(uint)EW_TYPE::NUM_EW_TYPE] = { nullptr };

	GameObject* selected_go = nullptr;

	bool SetMenuBar();
};