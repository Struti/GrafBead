#pragma once

#include "Camera.h"

namespace pndev {

	/*
		Abstract GUI element
	*/
	class GuiElement
	{
	public:
		GuiElement();
		~GuiElement();

		virtual void render(ti::Camera* camera) = 0;
	};

}