#include "MeshImporter.h"

void MeshImporter::Save(MeshData* mesh)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[2] = { mesh->num_index, mesh->num_vertex };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3;
	
	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;
	
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->num_index;
	memcpy(cursor, mesh->index, bytes);
	cursor += bytes;
}

void MeshImporter::Load(char* buffer, MeshData* mesh)
{
	char* cursor = buffer;

	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[5];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh->num_index = ranges[0];
	mesh->num_vertex = ranges[1];

	// Load indices
	/*bytes = sizeof(uint) * resource->num_indices;
	resource->indices = new uint[resource->num_indices];
	memcpy(resource->indices, cursor, bytes);
	cursor += bytes*/
}
