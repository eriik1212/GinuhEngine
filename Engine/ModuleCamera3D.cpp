#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;

	//LOG("Setting up the camera");
	App->menus->info.AddConsoleLog( "Setting up the camera");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	//LOG("Cleaning camera");
	App->menus->info.AddConsoleLog( "Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	//vec3 newPos(0,0,0);
	//vec3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 3.0f;

	//if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;


	// While Right clicking, ?WASD? fps-like movement and free look around must be enabled.
	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) sceneCam.frustum.pos += sceneCam.frustum.front * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) sceneCam.frustum.pos -= sceneCam.frustum.front * speed;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) sceneCam.frustum.pos -= sceneCam.frustum.WorldRight() * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) sceneCam.frustum.pos += sceneCam.frustum.WorldRight() * speed;

	// Scrool Zoom
	if (App->input->GetMouseZ() != 0) sceneCam.frustum.pos += sceneCam.frustum.front * (speed * 8.0f) * App->input->GetMouseZ();
	
	/*Position += newPos;
	Reference += newPos;*/

	//Focus On 
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if(App->scene_intro->gameobject_selected != nullptr)
			LookAt(App->scene_intro->GetGOGlobalPos(App->scene_intro->gameobject_selected));
		else
			LookAt(float3(0, 0, 0));
	}

	//----------------------------------------------------------------------- Rotate Camera While Right-Clicking (for FPS-UnityLikeMovement)

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.2f;

		Quat lookDirection;
		sceneCam.frustum.WorldMatrix().Decompose(float3(), lookDirection, float3());

		//Position -= Reference;

		// DeltaX
		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			Quat quatX = Quat::identity;

			quatX.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);
			lookDirection = quatX * lookDirection;
		}

		// DeltaY
		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			Quat quatY = Quat::identity;

			quatY.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), DeltaY * DEGTORAD);
			lookDirection = lookDirection * quatY;
		}

		// Set Rotation Matrix
		float4x4 rotationMatrix = sceneCam.frustum.WorldMatrix();
		rotationMatrix.SetRotatePart(lookDirection.Normalized());
		sceneCam.frustum.SetWorldMatrix(rotationMatrix.Float3x4Part());
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	sceneCam.frustum.pos = Position;
	reference = Reference;

	LookAt(reference);

	if(!RotateAroundReference)
	{
		reference = Position;
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	reference = Spot;

	reference = Spot;
	sceneCam.frustum.front = (Spot - sceneCam.frustum.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(sceneCam.frustum.front).Normalized();
	sceneCam.frustum.up = sceneCam.frustum.front.Cross(X);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	sceneCam.frustum.pos += Movement;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.ptr();
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	//ViewMatrixInverse = inverse(ViewMatrix);

	ViewMatrix = sceneCam.frustum.ViewMatrix();
	ViewMatrix.Transpose();
	
}
