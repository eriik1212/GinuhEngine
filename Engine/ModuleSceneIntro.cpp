#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "ConsoleInfo.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include "ModuleFilesManager.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	//LOG("Loading Intro assets");
	App->menus->info.AddConsoleLog( "Loading Intro assets");

	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	App->menus->info.AddConsoleLog( "Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	// ------------------------------------------------------------------- DRAW CUBE IN DIRECT MODE
	{
		/*
		glBegin(GL_QUADS);  // draw a cube with 12 triangles

		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glEnd();
	*/
	}

	// ------------------------------------------------------------------- DRAW CUBE IN VERTEX ARRAYS
	{
	/*
		//    v6----- v5
		//   /|      /|
		//  v1------v0|
		//  | |     | |
		//  | |v7---|-|v4
		//  |/      |/
		//  v2------v3

		uint my_id = 0,
			num_vertices = 36;

		GLfloat vertices[] = {
			// back face ===================
			1.0f, 1.0f, 1.0f,		// v0
			0.0f, 1.0f, 1.0f,		// v1
			0.0f, 0.0f, 1.0f,		// v2

			0.0f, 0.0f, 1.0f ,		// v2
			1.0f, 0.0f, 1.0f,		// v3
			1.0f, 1.0f, 1.0f,		// v0

			 // right face ===================
			 1.0f, 1.0f, 1.0f ,		// v0
			 1.0f, 0.0f, 1.0f ,		// v3
			 1.0f, 0.0f, 0.0f ,		// v4

			 1.0f, 0.0f, 0.0f ,		// v4
			 1.0f, 1.0f, 0.0f,		// v5
			 1.0f, 1.0f, 1.0f,		// v0

			 // top face ===================
			 1.0f, 1.0f, 1.0f,		// v0
			 1.0f, 1.0f, 0.0f,		// v5
		 	 0.0f, 1.0f, 0.0f,		// v6

			 0.0f, 1.0f, 0.0f,		// v6
			 0.0f, 1.0f, 1.0f,		// v1
			 1.0f, 1.0f, 1.0f ,		// v0

			 // left face ===================
			 0.0f, 1.0f, 1.0f,		// v1
			 0.0f, 1.0f, 0.0f,		// v6
			 0.0f, 0.0f, 0.0f,		// v7

			 0.0f, 0.0f, 0.0f,		// v7
			 0.0f, 0.0f, 1.0f,		// v2
			 0.0f, 1.0f, 1.0f,		// v1

			 // bottom face ================
			 0.0f, 0.0f, 1.0f ,		// v2
			 1.0f, 0.0f, 0.0f ,		// v4
			 1.0f, 0.0f, 1.0f ,		// v3

			 1.0f, 0.0f, 1.0f ,		// v3
			 0.0f, 0.0f, 1.0f ,		// v2
			 0.0f, 0.0f, 0.0f ,		// v7

			 // front face =================
			 (1.0f, 0.0f, 0.0f,		// v4
			 (0.0f, 0.0f, 0.0f,		// v7
			 (0.0f, 1.0f, 0.0f,		// v6

			 0.0f, 1.0f, 0.0f ,		// v6
			 1.0f, 1.0f, 0.0f ,		// v5
			 1.0f, 0.0f, 0.0f ,		// v4


		};
	// 36 of vertex coords

		/*glGenBuffers(1, (GLuint*)&(my_id));
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

		// activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		// draw a cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

		*/

	}

	// ------------------------------------------------------------------- DRAW CUBE IN INDEX
	{
		
		//    v6----- v5
		//   /|      /|
		//  v1------v0|
		//  | |     | |
		//  | |v7---|-|v4
		//  |/      |/
		//  v2------v3

		/*uint my_indices = 36,
			num_indices = 36;

		GLfloat vertices[] = { 
			1.0f, 1.0f, 1.0f, // v0
			0.0f, 1.0f, 1.0f, // v1
			0.0f, 0.0f, 1.0f, // v2
			1.0f, 0.0f, 1.0f, // v3
			1.0f, 0.0f, 0.0f, // v4
			1.0f, 1.0f, 0.0f, // v5
			0.0f, 1.0f, 0.0f, // v6
			0.0f, 0.0f, 0.0f, // v7

			};          // 8 of vertex coords

		GLubyte indices[] = { 0,1,2, 2,3,0,   // 36 of indices
							 0,3,4, 4,5,0,
							 0,5,6, 6,1,0,
							 1,6,7, 7,2,1,
							 7,4,3, 3,2,7,
							 4,7,6, 6,5,4 };

		//glGenBuffers(1, (GLuint*)&(my_indices));
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* num_indices, indices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		//glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

		// activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		// draw a cube
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, App->loader->textureId);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);*/
		
	}

	App->files_manager->Render();

	//FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

