#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	// Wireframe View
	if (wireframe)		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ------------------------------------------------------------------- DRAW CUBE IN DIRECT MODE
	{
		/*glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

	// bottom face =================
	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(0.0f, 0.0f, 1.0f);	// v1
	glVertex3f(0.0f, 0.0f, 0.0f);	// v0

	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(1.0f, 0.0f, 1.0f);	// v3
	glVertex3f(0.0f, 0.0f, 1.0f);	// v1

	// front face =================
	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(0.0f, 0.0f, 0.0f);	// v0
	glVertex3f(0.0f, 1.0f, 0.0f);	// v4


	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(0.0f, 1.0f, 0.0f);	// v4
	glVertex3f(1.0f, 1.0f, 0.0f);	// v5


	// right face ===================
	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(1.0f, 1.0f, 0.0f);	// v5
	glVertex3f(1.0f, 1.0f, 1.0f);	// v6


	glVertex3f(1.0f, 0.0f, 1.0f);	// v3
	glVertex3f(1.0f, 0.0f, 0.0f);	// v2
	glVertex3f(1.0f, 1.0f, 1.0f);	// v6

	// back face ===================
	glVertex3f(1.0f, 1.0f, 1.0f);	// v6
	glVertex3f(0.0f, 0.0f, 1.0f);	// v1
	glVertex3f(1.0f, 0.0f, 1.0f);	// v3

	glVertex3f(1.0f, 1.0f, 1.0f);	// v6
	glVertex3f(0.0f, 1.0f, 1.0f);	// v7
	glVertex3f(0.0f, 0.0f, 1.0f);	// v1

	// left face ===================
	glVertex3f(0.0f, 1.0f, 1.0f);	// v7
	glVertex3f(0.0f, 0.0f, 0.0f);	// v0
	glVertex3f(0.0f, 0.0f, 1.0f);	// v1

	glVertex3f(0.0f, 1.0f, 1.0f);	// v7
	glVertex3f(0.0f, 1.0f, 0.0f);	// v4
	glVertex3f(0.0f, 0.0f, 0.0f);	// v0

	// top face ===================
	glVertex3f(1.0f, 1.0f, 1.0f);	// v6
	glVertex3f(1.0f, 1.0f, 0.0f);	// v5
	glVertex3f(0.0f, 1.0f, 0.0f);	// v4


	glVertex3f(0.0f, 1.0f, 0.0f);	// v4
	glVertex3f(0.0f, 1.0f, 1.0f);	// v7
	glVertex3f(1.0f, 1.0f, 1.0f);	// v6


	glEnd );
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

		uint my_indices = 36,
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

		/*glGenBuffers(1, (GLuint*)&(my_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* num_indices, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);*/

		// activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		// draw a cube
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
		
	}

	return UPDATE_CONTINUE;
}

