#include "BoundingBox.h"

ti::BoundingBox::BoundingBox():
	BoundingBox(glm::vec3(0,0,0), glm::vec3(0, 0, 0))
{

}

ti::BoundingBox::BoundingBox(const glm::vec3& b, const glm::vec3& s):
	base_(b),
	sides_(s)
{

}


ti::BoundingBox::~BoundingBox()
{

}
glm::vec3 ti::BoundingBox::getSides()
{
	return sides_;
}

glm::vec3 ti::BoundingBox::getBasePoint()
{
	return base_;
}

bool ti::BoundingBox::contanisPoint(const glm::vec3& p)
{
	return p.x >= base_.x && p.x <= (base_.x + sides_.x) &&
		   p.y >= base_.y && p.y <= (base_.y + sides_.y) &&
		   p.z >= base_.z && p.z <= (base_.z + sides_.z);
}

void ti::BoundingBox::scaleToContain(const glm::vec3& p)
{
	if (base_.x > p.x) base_.x = p.x;
	if (base_.y > p.y) base_.y = p.y;
	if (base_.z > p.z) base_.z = p.z;

	if (base_.x + sides_.x < p.x) sides_.x = p.x - base_.x;
	if (base_.y + sides_.y < p.y) sides_.y = p.y - base_.y;
	if (base_.z + sides_.z < p.z) sides_.z = p.z - base_.z;
}

ti::BoundingBox ti::BoundingBox::getTransformedByModelMatrix(const glm::mat4& mm)
{
	glm::vec3 a[8];
	a[0] = base_;
	a[1] = base_ + glm::vec3(0,			0,			sides_.z);
	a[2] = base_ + glm::vec3(0,			sides_.y,	0);
	a[3] = base_ + glm::vec3(0,			sides_.y,	sides_.z);
	a[4] = base_ + glm::vec3(sides_.x,	0,			0);
	a[5] = base_ + glm::vec3(sides_.x,	0,			sides_.z);
	a[6] = base_ + glm::vec3(sides_.x,	sides_.y,	0);
	a[7] = base_ + glm::vec3(sides_.x,	sides_.y,	sides_.z);

	BoundingBox tr;
	tr.base_ = mm * glm::vec4(base_, 1);
	tr.sides_ = glm::vec3(0, 0, 0);
	for (int i = 1; i < 8; i++)
	{
		tr.scaleToContain(mm * glm::vec4(a[i], 1));
	}
	return tr;
}

void ti::BoundingBox::setBase(const glm::vec3& base)
{
	base_ = base;
}

void ti::BoundingBox::setSides(const glm::vec3& sides)
{
	sides_ = sides;
}
