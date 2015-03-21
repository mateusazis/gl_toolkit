#include "Mesh.h"
#include "Material.h"
#include "FBO.h"

Mesh::Mesh(GLenum topology, const float* vertices, int vCount, const int* indices, const int iCount, const float* uv)
: indexCount(iCount), topology(topology), hasIndices(indices != NULL), vCount(vCount)
{
	//generate VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//this.indices = indices;
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, vCount * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	//set posVBO on VAO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	if (uv){
		glGenBuffers(1, &uvVBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
		glBufferData(GL_ARRAY_BUFFER, vCount * 2 * sizeof(float), uv, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	if (indices){
		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(int), indices, GL_STATIC_DRAW);
		//this.elementsCount = indices.length;
	}
	/*this.attributes = {};
	if (!Mesh.COUNTS){
		Mesh.COUNTS = {};
		Mesh.COUNTS[gl.TRIANGLES] = 3;
		Mesh.COUNTS[gl.POINTS] = 1;
		Mesh.BASE = {}
		Mesh.BASE[gl.TRIANGLES] = 0;
		Mesh.BASE[gl.POINTS] = 0;
	};*/


	
	
	
}

void Mesh::draw(Material* mat, FBO* target){
	mat->commitQueue();

	//bind positions
	//int count = Mesh.BASE[this.topology] + Math.floor(this.vertexCount / Mesh.COUNTS[this.topology]);
	
	if (target)
		target->bind();
	//if (this.vaoHandle){


	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	
	if (indexCount != 0){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexVBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawArrays(topology, 0, vCount);
	}
	
	if (target)
		target->unbind();
}