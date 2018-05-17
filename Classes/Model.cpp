#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void printV(ti::Vertex v) {
	printf("(%0.2f, %0.2f, %0.2f) ", v.pos.x, v.pos.y, v.pos.z);
}

static std::string GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\") + 1);
	return "";
}

ti::Model::Model(Shader* shader, TextureLoader tl, std::string obj) :
	Object(shader)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	auto baseDir = GetBaseDir(obj);
	printf("Loading obj: %s [%s]", obj.c_str(), baseDir.c_str());

	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, obj.c_str(), baseDir.c_str()))
	{
		throw std::runtime_error(err);
	}

	printf("Errors:\n%s\n", err.c_str());

	printf("#materials = %d\n", materials.size());
	for (const auto& mat : materials) 
	{
		if (mat.diffuse_texname.size() > 0)
		{
			this->materials.push_back(tl.loadTexture(baseDir + "/" + mat.diffuse_texname));
		}
		else
		{
			this->materials.push_back({ 1,-1, -1,(GLuint)-1,0 });
		}
	}

	printf("shapes = %d\n", shapes.size());

	for (const auto& shape : shapes) 
	{
		RenderPart rp;
		rp.indexOffset = indices.size();
		rp.material = shape.mesh.material_ids.size() == 0 ? 0 : shape.mesh.material_ids[0];

		printf("shape.name = %s\n", shape.name.c_str());
		printf("\tshape.#indexes = %d\n", shape.mesh.indices.size());
		printf("\tshape.#materials = %d\n", shape.mesh.material_ids.size());
		printf("\tshape.#nfv = %d\n", shape.mesh.num_face_vertices.size());
		printf("\tshape.#indexes/nfv = %f\n", 1.0f*shape.mesh.indices.size()/shape.mesh.num_face_vertices.size());

		for (auto i = 0; i < shape.mesh.indices.size(); i++) 
		{
			auto index = shape.mesh.indices[i];

			Vertex vertex;
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1- attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};
			vertex.color = glm::vec3(materials[rp.material].diffuse[0], materials[rp.material].diffuse[1], materials[rp.material].diffuse[2]);

			vertices.push_back(vertex.pos);
			texcoords.push_back(vertex.texCoord);
			normals.push_back(vertex.normal);
			colors.push_back(vertex.color);
			indices.push_back(indices.size());
		}

		rp.indexCount = indices.size() - rp.indexOffset;

		renderParts.push_back(rp);
	}
}

void ti::Model::initialize()
{
	GLuint vbo, tbo, nbo, cbo, ebo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void ti::Model::update()
{

}

void ti::Model::renderInternal()
{
	glUniform1i(shader_->getUniformLocation("object_texture"), 0);

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);

	for (const auto& rp : renderParts)
	{
		if (materials[rp.material].texture_id == (GLuint)-1)
		{
			glUniform1i(shader_->getUniformLocation("no_texture"), 1);
		}
		else 
		{
			glUniform1i(shader_->getUniformLocation("no_texture"), 0);
			glBindTexture(GL_TEXTURE_2D, materials[rp.material].texture_id);
		}
		glDrawElements(GL_TRIANGLES, rp.indexCount, GL_UNSIGNED_INT, (void*)(rp.indexOffset * sizeof(unsigned int)));
	}		
	glBindVertexArray(0);
}
