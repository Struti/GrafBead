#include "RenderScene.h"
#include "Object.h"

ti::RenderScene* ti::RenderScene::active_camerascene_ = nullptr;

ti::RenderScene* ti::RenderScene::getActiveScene()
{
	return active_camerascene_;
}

ti::RenderScene::RenderScene(std::string name) :
	scene_name_(name),
	active_camera_(nullptr),
	sky(nullptr),
	water(nullptr),
	light(nullptr),
	shore_(nullptr)
{
	if (active_camerascene_ == nullptr)
		active_camerascene_ = this;
}

ti::RenderScene::~RenderScene()
{
	if (active_camerascene_ == this)
		active_camerascene_ = nullptr;
}

void ti::RenderScene::initialize(const ti::SceneSettings& settings)
{
	active_camerascene_ = this;
}

void ti::RenderScene::update()
{
	for (auto object : objects_) 
	{
		object->update();
	}
}

void ti::RenderScene::setSky(SkyBox* sky)
{
	this->sky = sky;
}

void ti::RenderScene::setLight(DirectionalLight* light)
{
	this->light = light;
}

void ti::RenderScene::setInfiniteShore(InfiniteShore* shore)
{
	this->shore_ = shore;
	this->shore_->initialize();
	this->objects_.push_back(shore);
}

void ti::RenderScene::render()
{
	if (active_camera_ == nullptr)
	{
		return;
	}

	FboManager::unbindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (sky != nullptr)
	{
		sky->renderSkybox(*active_camera_);
	}

	auto campos = active_camera_->getPosition();

	shore_->setPosition(glm::floor(campos.x), 0, glm::floor(campos.z));

	for (auto object : objects_)
	{
		object->getShader()->use();

		auto wl = object->getShader()->getUniformLocation("water_level");
		auto refl = object->getShader()->getUniformLocation("reflectionRender");
		auto refr = object->getShader()->getUniformLocation("refractionRender");

		glUniform1f(wl, water != nullptr ? water->waterlevel : 10);
		glUniform1i(refl, 0);
		glUniform1i(refr, 0);

		object->render(active_camera_, light);
	}

	if (water != nullptr)
	{
		int w, h;
		active_camera_->getViewPort(w, h);

		glClearColor(0, 0, 0, 0);
		water->waterRefraction.bindFBO();

		for (auto object : objects_)
		{
			object->getShader()->use();

			auto wl = object->getShader()->getUniformLocation("water_level");
			auto refl = object->getShader()->getUniformLocation("reflectionRender");
			auto refr = object->getShader()->getUniformLocation("refractionRender");

			glUniform1f(wl, water->waterlevel);
			glUniform1i(refl, 0);
			glUniform1i(refr, 1);

			object->render(active_camera_, light);
		}

		glCullFace(GL_FRONT);

		water->waterReflection.bindFBO();

		for (auto object : objects_)
		{
			object->getShader()->use();

			auto refl = object->getShader()->getUniformLocation("reflectionRender");
			auto refr = object->getShader()->getUniformLocation("refractionRender");

			glUniform1i(refl, 1);
			glUniform1i(refr, 0);

			object->render(active_camera_, light);
		}
		glCullFace(GL_BACK);

		FboManager::unbindFBO();

		glViewport(0, 0, w, h);
		water->skytexture = sky->skybox.texture_id;

		water->setPosition(glm::floor(campos.x), 0, glm::floor(campos.z));
		water->render(active_camera_, light);
	}
}

void ti::RenderScene::setActiveCamera(int index)
{
	if (index >= 0 && index < cameras_.size())
	{
		active_camera_ = cameras_[index];
	}
}

void ti::RenderScene::setWater(ti::Water * water)
{
	this->water = water;
	this->water->initialize();
}

void ti::RenderScene::addCamera(ti::Camera* camera)
{
	if (camera != nullptr)
	{
		cameras_.push_back(camera);

		if (active_camera_ == nullptr)
		{
			active_camera_ = camera;
		}
	}
}

void ti::RenderScene::addObject(ti::Object* obj)
{
	if (obj != nullptr)
	{
		obj->initialize();
		objects_.push_back(obj);
	}
}

ti::Camera* ti::RenderScene::getActiveCamera()
{
	return active_camera_;
}

ti::Camera* ti::RenderScene::getCamera(int index)
{
	if (index < cameras_.size() && index >= 0)
		return cameras_[index];
	return nullptr;
}

std::vector<ti::Camera*> ti::RenderScene::getCameras()
{
	return cameras_;
}
