#pragma once
#include <vector>
#include <string>

#include "Camera.h"
#include "Object.h"
#include "Water.h"
#include "SkyBox.h"
#include "InfiniteShore.h"

namespace ti
{
	struct SceneSettings
	{
		glm::ivec2 shadowmapResolution;
	};

	/*
		A scene that holds the objects and gui to be rendered.
		Supports light, shadows, skybox, multiple cameras.
	*/
	class RenderScene
	{
	public:

		/*
			Retrieves the current active scene. It is used for rendering.
		*/
		static RenderScene* getActiveScene();

		RenderScene(std::string name);
		~RenderScene();

		void initialize(const SceneSettings& settings);

		void update();
		void render();

		void setActiveCamera(int cameraIndex);
		void setWater(Water* water);
		void setSky(SkyBox* sky);
		void setLight(DirectionalLight* light);
		void setInfiniteShore(InfiniteShore* shore);

		void addCamera(Camera* camera);
		void addObject(Object* obj);

		Camera* getActiveCamera();
		Camera* getCamera(int index);
		std::vector<Camera*> getCameras();

	private:

		static RenderScene* active_camerascene_;

		Camera* active_camera_;

		Water* water;
		SkyBox* sky;
		DirectionalLight* light;
		InfiniteShore* shore_;

		std::string scene_name_;
		std::vector<Object*> objects_;
		std::vector<Camera*> cameras_;
	};
}