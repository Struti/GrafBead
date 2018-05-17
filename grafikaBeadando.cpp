// grafikaBeadando.cpp : Defines the entry point for the console application.
//
#define GLEW_STATIC

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <string>
#include <chrono>

#include "Classes\Camera.h"
#include "Classes\Control.h"
#include "Classes\RenderScene.h"
#include "Classes\TimeCalculator.h"
#include "Classes\HeightMapLoader.h"
#include "Classes\Water.h"
#include "Classes\SkyBox.h"
#include "Classes\Model.h"


static const float PI = 3.141592f;

struct WindowContextSettings {

	std::string title;
	int width;
	int height;
	int oglMajorVersion;
	int oglMinorVersion;
	bool wireframe;

	WindowContextSettings()
	{
		this->title = "Grafika Beadando";
		this->width = 1024;
		this->height = 768;
		this->oglMajorVersion = 4;
		this->oglMinorVersion = 5;
		this->wireframe = false;
	}
};

static WindowContextSettings windowSettings;

static void framebufferSize(GLFWwindow* window, int width, int height)
{
	windowSettings.width = width;
	windowSettings.height = height;
}

static void error(int error, const char* description)
{
	std::cerr << "Error code " << error << ": " << description << "\n";
};

int main(void)
{
	glfwSetErrorCallback(error);

	if (GLFW_TRUE != glfwInit())
	{
		return -1;
	}

	std::cout << "GLFW initialized\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowSettings.oglMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowSettings.oglMinorVersion);

	GLFWwindow* window = glfwCreateWindow(windowSettings.width, windowSettings.height, windowSettings.title.c_str(), NULL, NULL);

	if (!window)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);


	const GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << "\n";
		return -1;
	}

	std::cout << "GLEW initialized\n";

	std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL version: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	std::cout << "OpenGL vendor: " << (char*)glGetString(GL_VENDOR) << "\n";

	glfwSetFramebufferSizeCallback(window, framebufferSize);
	glfwSetKeyCallback(window, ti::Controller::keyMapping);
	glfwSetCursorPosCallback(window, ti::Controller::cursorPosition);
	glfwSetMouseButtonCallback(window, ti::Controller::mouseButtonCallback);

	ti::Controller::cameraState.cameraSpeed = 50.0f;

	ti::SceneSettings sceneSettings;
	sceneSettings.shadowmapResolution = glm::ivec2(1024, 1024);
	ti::RenderScene scene("demo");

	scene.initialize(sceneSettings);

	ti::TextureLoader textureLoader;

	ti::Camera camera;
	scene.addCamera(&camera);
	scene.setActiveCamera(0);
	
	ti::Shader terrainShader;
	ti::Shader waterShader;
	ti::Shader skyShader;
	ti::Shader objShader;
	ti::Shader shoreShader;

	terrainShader.load("Shaders/Terrain/terrainVertex.txt", "Shaders/Terrain/terrainFragment.txt", 0, 0);
	waterShader.load("Shaders/Water/waterVertex.txt", "Shaders/Water/waterFragment.txt", 0, 0);
	skyShader.load("Shaders/Sky/skyvert.glsl", "Shaders/Sky/skyfrag.glsl", 0, 0);
	objShader.load("Shaders/Object/objVertex.txt", "Shaders/Object/objFragment.txt", 0, 0);
	shoreShader.load("Shaders/Shore/shoreVertex.glsl", "Shaders/Shore/shoreFragment.glsl", 0, 0);

	auto skybox = textureLoader.loadCubemap(
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayRight2048.png",
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayLeft2048.png",
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayUp2048.png",
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayDown2048.png",
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayBack2048.png",
			"Texture/Skybox/SkyboxSet/Sunny/TropicalSunnyDayFront2048.png");

	auto terr_texture1 = textureLoader.loadTexture("Texture/Terrain/grass.jpg");
	auto terr_texture2 = textureLoader.loadTexture("Texture/Terrain/grasstosand.jpg");
	auto terr_texture3 = textureLoader.loadTexture("Texture/Terrain/rock.jpg");
	auto terr_shore = textureLoader.loadTexture("Texture/Terrain/sand.jpg");

	auto normal_map = textureLoader.loadTexture("Texture/Water/normal.BMP");
	auto foam = textureLoader.loadTexture("Texture/Water/foam.BMP");

	ti::HeightMap heightMap(terrainShader, "HeightMaps/island2.png", 50, terr_texture1, terr_texture2, terr_texture3, terr_shore);
	ti::Water water(waterShader, 600, 600, normal_map, foam);
	ti::SkyBox sky(skyShader, skybox);
	ti::DirectionalLight sun;
	ti::InfiniteShore shore(shoreShader, terr_shore, 400);

	sun.setDirection(glm::vec3(0.5, -0.7, 0.4));
	sun.setDiffuseColor(glm::vec3(0.9, 0.8, 0.7));
	sun.setAmbientColor(glm::vec3(0.2, 0.2, 0.3));

	ti::Model boat(&objShader, textureLoader, "Models/m2un5jnd8d-cruisership2012/Cruisership 2012/Cruiser 2012.obj");
	boat.setPosition(-1000, 5, 1000);
	boat.setScale(0.2, 0.2, 0.2);

	scene.addObject(&heightMap);
	scene.addObject(&boat);
	scene.setWater(&water);
	scene.setSky(&sky);
	scene.setLight(&sun);
	scene.setInfiniteShore(&shore);

	camera.setPosition(glm::vec3(100, 100, 100));
	camera.lookAt(glm::vec3(0, 0, 0));
	camera.setParameters(60, 1, 250);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glfwSwapInterval(0);

	ti::CalculateTime::init();

	// windowSettings.wireframe = true;

	ti::Controller::light = &sun;

	while (!glfwWindowShouldClose(window))
	{
		ti::CalculateTime::calculateFrameTime();

		if (windowSettings.wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}

		ti::Controller::cursorPositionUpdate();

		camera.update(windowSettings.width, windowSettings.height);

		ti::Controller::cameraMovement(scene.getActiveCamera(), ti::Controller::cameraState.movementState, (float)ti::CalculateTime::getFrameDelta(), ti::Controller::cameraState.cameraSpeed);
		ti::Controller::cameraRotation(scene.getActiveCamera());

		boat.setRotation(0, 0, 1, sin(ti::CalculateTime::getElapsedTime()) * 0.25);

		// nap forog
		sun.setDirection(glm::vec3(sin(ti::CalculateTime::getElapsedTime() * 0.2), -0.5, cos(ti::CalculateTime::getElapsedTime() * 0.2)));

		scene.update();
		scene.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}