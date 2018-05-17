#include "SkyBox.h"

#include "TextureLoader.h"
#include "Camera.h"
#include "DirectionalLight.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ti::SkyBox::SkyBox(
	const Shader& skyshader,
	const Texture& skybox) :
	skybox_shader_(skyshader),
	skybox(skybox)
{
	loadSkybox();
}

ti::SkyBox::~SkyBox()
{

}

void ti::SkyBox::loadSkybox()
{
	float vertices[] = {
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	unsigned int indices[] = {
		2, 1, 0,
		0, 3, 2,
		6, 5, 1,
		1, 2, 6,
		5, 6, 7,
		7, 4, 5,
		3, 0, 4,
		4, 7, 3,
		1, 5, 4,
		4, 0, 1,
		6, 2, 3,
		3, 7, 6,
	};

	GLuint vbo;
	GLuint ebo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &cube_vao_);

	glBindVertexArray(cube_vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void ti::SkyBox::renderSkybox(const Camera& camera)
{
	glDepthMask(GL_FALSE);

	skybox_shader_.use();

	float near, f, fy;
	camera.getParameters(fy, near, f);

	GLuint skycubemap = skybox_shader_.getUniformLocation("skybox_texture");

	GLuint pmu = skybox_shader_.getUniformLocation("projection_matrix");
	GLuint vmu = skybox_shader_.getUniformLocation("view_matrix");

	glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()))*glm::scale(glm::mat4(), 0.5f*glm::vec3(f, f, f));
	glUniformMatrix4fv(vmu, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(pmu, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

	glUniform1i(skycubemap, 0);
	
	glBindVertexArray(cube_vao_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture_id);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}
