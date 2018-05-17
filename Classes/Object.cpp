#include "Object.h"

int ti::Object::ID_AUTO_INCR_ = 0;

ti::Object::Object(Shader* shader):
	shader_(shader),
	id_(ID_AUTO_INCR_),
	position_(glm::vec3(0, 0, 0)),
	rotation_(glm::quat()),
	scale_(glm::vec3(1, 1, 1))

{
};

void ti::Object::setRotation(const glm::quat& quaternion)
{
	rotation_ = glm::normalize(quaternion);
};

void ti::Object::setRotation(float x, float y, float z, float angle)
{
	rotation_ = glm::angleAxis(glm::radians(angle), glm::vec3(x, y, z));
};

void ti::Object::setPosition(float x, float y, float z)
{
	position_.x = x;
	position_.y = y;
	position_.z = z;
};

void ti::Object::setScale(float x, float y, float z)
{
	scale_.x = x;
	scale_.y = y;
	scale_.z = z;
}
void ti::Object::setShader(Shader * shader)
{
	shader_ = shader;
}

glm::quat ti::Object::getRotation()
{
	return rotation_;
};

glm::vec3 ti::Object::getPosition()
{
	return position_;
}

glm::vec3 ti::Object::getScale()
{
	return scale_;
}

ti::Shader* ti::Object::getShader()
{
	return shader_;
}

glm::mat4 ti::Object::getModelMatrix()
{
	glm::mat4 S = glm::scale(glm::mat4(), scale_);
	S[3][3] = 1;
	glm::mat4 R = glm::toMat4(rotation_);
	glm::mat4 invR = glm::transpose(R);
	glm::vec4 t = glm::vec4(position_.x, position_.y, position_.z, 1)*invR;
	glm::vec3 t3(t.x, t.y, t.z);
	return glm::translate(R*S, t3);
};

void ti::Object::render(ti::Camera* camera, ti::DirectionalLight* light)
{
	if (shader_ != nullptr && camera != nullptr)
	{
		shader_->use();

		GLuint mmu = shader_->getUniformLocation("model_matrix");
		GLuint vmu = shader_->getUniformLocation("view_matrix");
		GLuint pmu = shader_->getUniformLocation("projection_matrix");
		GLuint cps = shader_->getUniformLocation("camera_position");

		if (light != nullptr) 
		{
			GLuint lightDir = shader_->getUniformLocation("light_direction");
			GLuint lightColorD = shader_->getUniformLocation("light_diffuse");
			GLuint lightColorA = shader_->getUniformLocation("light_ambient");
			glUniform3f(lightDir, light->getDirection().x, light->getDirection().y, light->getDirection().z);
			glUniform3f(lightColorD, light->getDiffuseColor().r, light->getDiffuseColor().g, light->getDiffuseColor().b);
			glUniform3f(lightColorA, light->getAmbientColor().r, light->getAmbientColor().g, light->getAmbientColor().b);
		}

		glUniform3f(cps, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
		glUniformMatrix4fv(mmu, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		glUniformMatrix4fv(vmu, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(pmu, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

		renderInternal();
	}
}

void ti::Object::render(Shader* customShader, const glm::mat4& view, const glm::mat4& projection, ti::DirectionalLight* light)
{
	if (customShader != nullptr)
	{
		customShader->use();

		GLuint mmu = glGetUniformLocation(customShader->getProgram(), "model_matrix");
		GLuint vmu = glGetUniformLocation(customShader->getProgram(), "view_matrix");
		GLuint pmu = glGetUniformLocation(customShader->getProgram(), "projection_matrix");

		glUniformMatrix4fv(mmu, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		glUniformMatrix4fv(vmu, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(pmu, 1, GL_FALSE, glm::value_ptr(projection));

		if (light != nullptr)
		{
			GLuint lightDir = shader_->getUniformLocation("light_direction");
			GLuint lightColorD = shader_->getUniformLocation("light_diffuse");
			GLuint lightColorA = shader_->getUniformLocation("light_ambient");
			glUniform3f(lightDir, light->getDirection().x, light->getDirection().y, light->getDirection().z);
			glUniform3f(lightColorD, light->getDiffuseColor().r, light->getDiffuseColor().g, light->getDiffuseColor().b);
			glUniform3f(lightColorA, light->getAmbientColor().r, light->getAmbientColor().g, light->getAmbientColor().b);
		}

		renderInternal();
	}
}

