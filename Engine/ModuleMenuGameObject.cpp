#include "Globals.h"
#include "Application.h"
#include "ModuleMenuGameObject.h"
#include "ModuleMenus.h"

ModuleMenuGameObject::ModuleMenuGameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleMenuGameObject::~ModuleMenuGameObject()
{}

// Load assets
bool ModuleMenuGameObject::Start()
{
	//LOG("Loading Intro assets");
	App->menus->info.AddConsoleLog("Loading Menu Game Object");

	bool ret = true;

	return ret;
}

// Load assets
bool ModuleMenuGameObject::CleanUp()
{
	//LOG("Unloading Intro scene");
	App->menus->info.AddConsoleLog("Unloading Menu Game Object");

	return true;
}

GameObject ModuleMenuGameObject::CreateGameObject(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	GameObject* newParent = nullptr;

	bool necessaryNode = node->mNumChildren > 1;


	//if (parent == nullptr) returnGameObject = newParent = new GameObject(rootGameObject, "GameObject");
	//else newParent = new GameObject(parent, node->mName.C_Str());

	// Set new GameObject position with node Transform.
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos, { 1.0f, 1.0f, 1.0f }, eulerRot);


	return newParent;
}

// Update: draw background
update_status ModuleMenuGameObject::Update(float dt)
{

	
	return UPDATE_CONTINUE;
}