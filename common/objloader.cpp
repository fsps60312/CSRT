#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::ivec3>& out_vertex_ids,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::ivec3>& out_uv_ids,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::ivec3>& out_normal_ids
){
	out_vertices.resize(1);
	out_uvs.resize(1);
	out_normals.resize(1);

	printf("Loading OBJ file %s...\n", path);



	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			out_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			out_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			out_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			glm::ivec3 vertex_id,uv_id,normal_id;
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_id.x, &uv_id.x, &normal_id.x, &vertex_id.y, &uv_id.y, &normal_id.y, &vertex_id.z, &uv_id.z, &normal_id.z );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			out_vertex_ids.push_back(vertex_id);
			out_uv_ids.push_back(uv_id);
			out_normal_ids.push_back(normal_id);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}
	fclose(file);
	return true;
}