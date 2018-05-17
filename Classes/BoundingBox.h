#pragma once

#include <glm\glm.hpp>

namespace ti 
{
	/*
		Axis aligned bounding box (AABB)
	*/
	class BoundingBox
	{
	public:

		/*
			Constructs a zero-sized center positioned bounding box.
		*/
		BoundingBox();
		
		/*
			Creates a minimal volume AABB that contains the points 'base' and 'base+sides'.
		*/
		BoundingBox(const glm::vec3& base, const glm::vec3& sides);
		~BoundingBox();

		glm::vec3 getSides();
		glm::vec3 getBasePoint();

		bool contanisPoint(const glm::vec3& p);

		/*
			Calculates the minimal volume AABB that contains *this and the given point.
		*/
		void scaleToContain(const glm::vec3& p);

		/*
			Calculates the minimal volume AABB that contains this AABB transformed by the parameter.
		*/
		BoundingBox getTransformedByModelMatrix(const glm::mat4& mm);

		void setBase(const glm::vec3& base);
		void setSides(const glm::vec3& sides);

	private:

		glm::vec3 base_;
		glm::vec3 sides_;

	};

}