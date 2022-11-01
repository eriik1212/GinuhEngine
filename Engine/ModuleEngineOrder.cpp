#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "EO_Game.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "C_MeshRenderer.h"

ModuleEO::ModuleEO(bool start_enabled) : Module(start_enabled)
{
}

ModuleEO::~ModuleEO()
{
}

bool ModuleEO::Init()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");
    return true;
}

bool ModuleEO::Start()
{
    engine_order[(uint)EO_NUM::EDITOR] = editor = new EO_Editor();
    editor->SetSelectedGameObject(game_objects[2]);
    engine_order[(uint)EO_NUM::GAME] = new EO_Game();

    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->Start();
        }
    }

    return true;
}

update_status ModuleEO::PreUpdate(float dt)
{
    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->PreUpdate();
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleEO::Update(float dt)
{
    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->Update();
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleEO::PostUpdate(float dt)
{
    return UPDATE_CONTINUE;
}

void ModuleEO::DrawEO()
{
    for (int i = 0; i < (uint)EO_NUM::EDITOR; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->PostUpdate();
        }
    }
}

void ModuleEO::DrawEO_Editor()
{
    if (engine_order[(uint)EO_NUM::EDITOR] && engine_order[(uint)EO_NUM::EDITOR]->IsEnabled())
    {
        engine_order[(uint)EO_NUM::EDITOR]->PostUpdate();
    }
}

bool ModuleEO::CleanUp()
{
    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i])
        {
            engine_order[i]->CleanUp();
        }
        RELEASE(engine_order[i]);
    }
    RELEASE(rootGameObject);
    return true;
}

