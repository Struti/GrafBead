#pragma once

#include "GLIncludes.h"

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "DirectionalLight.h"

namespace ti
{
	/*
		It holds the general transform of a 3D object (position, scale, orientation).
		Internal rendering, initialize and update methods must be implemented.
	*/
	class Object
	{
	public:

		Object(Shader* shader);

		void setRotation(const glm::quat& quaternion);
		void setRotation(float x, float y, float z, float angle);
		void setPosition(float x, float y, float z);
		void setScale(float x, float y, float z);

		void setShader(Shader* shader);

		glm::quat getRotation();
		glm::vec3 getPosition();
		glm::vec3 getScale();

		Shader* getShader();

		/*
			Calculates the model matrix from the given position, scale and orientation.
		*/
		glm::mat4 getModelMatrix();

		void render(ti::Camera* camera, ti::DirectionalLight* light);

		void render(Shader* customShader, const glm::mat4& view, const glm::mat4& projection, ti::DirectionalLight* light);

		/*
			Initializes the object.
		*/
		virtual void initialize() = 0;

		/*
			Updates the state of the object.
		*/
		virtual void update() = 0; 
	
	protected:
			
		//BoundingBox* bounding_box_; // the original aabb in model space

		glm::vec3 position_;
		glm::vec3 scale_;
		glm::quat rotation_;

		Shader* shader_;
		virtual void renderInternal() = 0;

	private:
		//BoundingBox transformed_aabb_; // transformed by model_matrix, for internal use only
		static int ID_AUTO_INCR_;
		int id_;
	};
}