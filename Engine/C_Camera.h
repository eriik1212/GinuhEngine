#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct MeshData;

class C_Camera : public Component
{
public:
	C_Camera();
	C_Camera(GameObject* gameObject);
	~C_Camera();

	void Update() override;

	void PrintGui();

	string name;
	MeshData* mesh;

	Frustum frustum;

	//void SetAspectRatio(float aspectRatio);

	//------------------------------- Frame Buffer Logics
	void InitFrameBuffer();

	unsigned int frameBuff;
	unsigned int textColorBuff;
	unsigned int rbo;

};