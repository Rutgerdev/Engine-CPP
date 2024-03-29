//World , components , managers
#include "LevelManager.h"
#include "mge/core/World.hpp"
#include "mge/core/collision/PhysicsWorld.h"
#include "mge/core/Mesh.hpp"
#include "mge/LUA/LUAManager.h"
#include "mge/core/Camera.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/xml/XmlReader.h"
#include "mge/core/SoundManager.h"
#include "mge/util/DebugHud.hpp"
//Lights and gameobjects
#include "mge/core/GameObject.hpp"
#include "mge/core/Player.h"
#include "mge/core/collision/RigidbodyGameObject.h"
#include "mge/core/collision/StaticGameObject.h"
#include "mge/core/collision/BoxTrigger.h"
#include "mge/core/collision/Collision.h"
#include "mge/core/light/Light.h"
#include "mge/core/light/DirectionalLight.h"
#include "mge/core/light/PointLight.h"
#include "mge/core/light/SpotLight.h"

//Additional materials
#include "mge/materials/TextureNormalMaterial.hpp"
#include "mge/materials/BasicTextureLight.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureLitMaterial.hpp"
#include "mge/core/FadeManager.h"
//Behaviours 
#include "mge/behaviours/StatueBehaviour.h"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour2.hpp"
#include "mge/behaviours/FPController.h"
#include "mge/behaviours/BoxBehaviour.h"
#include "mge/behaviours/DoorBehaviour.h"
#include "mge/behaviours/TriggerBehaviour.h"
#include "mge/behaviours/FPCamera.h"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/Orbit.hpp"
#include "SFML\Audio.hpp"
//Util
#include "mge/util/Color.h"
#include "mge/config.hpp"
#include "mge\core\Timer.hpp"
#include "mge/SubtitleManager.h"

LevelManager::LevelManager() 
{
	cout << "Current level at start -> " << currentlevel << endl;
}

LevelManager::~LevelManager()
{

}

void LevelManager::SwitchToLevel(GameLevels pToLevel)
{
	if (currentlevel == pToLevel)
		return;



	//_hud->setFade(true);
	_hud->showLoadingScreen();

	DestroyLevel(currentlevel);
	std::cout << "destroyed level" << currentlevel  <<std::endl;
	
	BuildLevel(pToLevel);
	currentlevel = pToLevel;
	_hud->disableLoadingScreen();
	//_hud->setFade(false);
}
void LevelManager::ReloadLevel()
{
	DestroyLevel(currentlevel);
	BuildLevel(currentlevel);
}
void LevelManager::BuildLevel(GameLevels pLevel)
{
	//_world->CreateTriggerManager();
	switch (pLevel)
	{
	case Menu:
		Build_menu();
		break;
	case HUB:
		Build_level_hub();
		break;
	case HUBTUTORIAL:
		Build_level_hub_tutorial();
		break;
	case Level1:
		Build_level_1();
		break;
	case Level2:
		Build_level_2();
		break;
	case Level3:
		Build_level_3();
		break;
	case Level4:
		Build_level_4();
		break;
	default:
		break;
	}
}

void LevelManager::DestroyLevel(GameLevels pLevel)
{
	switch (pLevel)
	{
	case Menu:
		cout << " Menu destroyed " << endl;

		_world->CleanUpPhysicsWorld();
		//cleanup menu
		break;
	case HUB:
		cout << " HUB destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		// Do something special so hub doesnt get destroyed completely
		// or maybe store info in a class and use that info to create it again (so doors are closed after you complete level 1 )
		break;

	case HUBTUTORIAL:
		cout << " HUB destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		// Do something special so hub doesnt get destroyed completely
		// or maybe store info in a class and use that info to create it again (so doors are closed after you complete level 1 )
		break;
	case Level1:
		cout << " Level 1 destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		break;
	case Level2:
		cout << " Level 2 destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		break;
	case Level3:
		cout << " Level 3 destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		break;
	case Level4:
		cout << " Level 4 destroyed " << endl;
		_world->CleanUpPhysicsWorld();
		break;
	default:
		break;
	}
}

void LevelManager::Build_menu()
{
	cout << " Menu Build " << endl;
}
float subStartTime = 0;
float lastSubTime = 0;
bool tutorialStart = false;


void LevelManager::Build_level_hub()
{
	
	StaticGameObject * coffins = new StaticGameObject("coffins", glm::vec3(0), _world);
	coffins->setMesh(Mesh::load(config::MGE_MODEL_PATH + "Coffins.obj"));
	coffins->setMaterial(new BasicTextureLit(Texture::load(config::MGE_TEXTURE_PATH + "coffin_DIFF(TEMP).png"), 0.1f));

	_world->add(coffins);

	StaticGameObject * exitBlock = new StaticGameObject("exitblock", glm::vec3(0), _world);
	exitBlock->setMesh(Mesh::load(config::MGE_MODEL_PATH + "Exit_Block.obj"));
	exitBlock->setMaterial(new BasicTextureLit(Texture::load(config::MGE_TEXTURE_PATH + "step_DIFF.png"), 0.1f));

	_world->add(exitBlock);

	XmlReader * xmlReader = new XmlReader(_world);
	xmlReader->LoadLevel("level_hub");
	xmlReader->LoadInteractables("interactables_level_hub2");
	xmlReader->LoadSubtitleTriggers("triggers_level_hub");

	
	LUAManager::InitializeFile(_world,"level_hub");
}

void LevelManager::Build_level_hub_tutorial()
{
	StaticGameObject * coffins = new StaticGameObject("coffins", glm::vec3(0), _world);
	coffins->setMesh(Mesh::load(config::MGE_MODEL_PATH + "Coffins.obj"));
	coffins->setMaterial(new BasicTextureLit(Texture::load(config::MGE_TEXTURE_PATH + "coffin_DIFF(TEMP).png"), 0.1f));

	_world->add(coffins);

	StaticGameObject * exitBlock = new StaticGameObject("exitblock", glm::vec3(0), _world);
	exitBlock->setMesh(Mesh::load(config::MGE_MODEL_PATH + "Exit_Block.obj"));
	exitBlock->setMaterial(new BasicTextureLit(Texture::load(config::MGE_TEXTURE_PATH + "step_DIFF.png"), 0.1f));

	_world->add(exitBlock);

	XmlReader * xmlReader = new XmlReader(_world);
	xmlReader->LoadLevel("level_hub");
	xmlReader->LoadInteractables("interactables_level_hub");
	xmlReader->LoadSubtitleTriggers("triggers_level_hub");

	LUAManager::InitializeFile(_world, "level_hub");

	delete xmlReader;
	subStartTime = Timer::now();
	tutorialStart = true;
}

sf::SoundBuffer ambientBuffer;
sf::Sound soundAmbient;

void LevelManager::Build_level_1()
{	
	cout << " Build level 1 " << endl;

	XmlReader * xmlReader = new XmlReader(_world);
	xmlReader->LoadLevel("level_01");
	xmlReader->LoadInteractables("interactables_level_01");
	xmlReader->LoadSubtitleTriggers("triggers_level_01");
	LUAManager::InitializeFile(_world,"level_01");
	
	//if (!ambientBuffer.loadFromFile(config::MGE_SOUND_PATH + "ambience.wav"))
	//{
	//	std::cout << " cant load sound " << std::endl;
	//}
	//soundAmbient.setBuffer(ambientBuffer);
	//soundAmbient.setLoop(true);
	//soundAmbient.play();
	delete xmlReader;
}

void LevelManager::Build_level_2()
{
	StaticGameObject * bridge = new StaticGameObject("bridge", glm::vec3(0), _world);
	bridge->setMesh(Mesh::load(config::MGE_MODEL_PATH + "bridge_lvl2.obj"));
	bridge->setMaterial(new TextureLitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "brokenbridge_DIFF.png"), Texture::load(config::MGE_TEXTURE_PATH + "brokenbridge_NRM.png"), 0.1f));
	

	_world->add(bridge);

	XmlReader * xmlReader = new XmlReader(_world);

	xmlReader->LoadLevel("level_02");
	xmlReader->LoadInteractables("interactables_level_02");
	xmlReader->LoadSubtitleTriggers("triggers_level_02");
	LUAManager::InitializeFile(_world,"level_02");



	delete xmlReader;
}

void LevelManager::Build_level_3()
{
	XmlReader * xmlReader = new XmlReader(_world);
	xmlReader->LoadLevel("level_03");
	xmlReader->LoadInteractables("interactables_level_03");
	xmlReader->LoadSubtitleTriggers("triggers_level_03");
	//LUAManager::InitializeFile(_world);

	delete xmlReader;
}

void LevelManager::Build_level_4()
{

	XmlReader * xmlReader = new XmlReader(_world);
	xmlReader->LoadLevel("level_04");
	xmlReader->LoadInteractables("interactables_level_04");
	xmlReader->LoadSubtitleTriggers("triggers_level_04");
	//LUAManager::InitializeFile(_world);

	std::cout << "LOADED level 4 bIATIHT" << std::endl;

	//delete xmlReader;
}
bool s_1 = false;
bool s_2 = false;
bool s_3 = false;
bool s_4 = false;
bool s_5 = false;
bool s_6 = false;

void LevelManager::testUpdate()
{




	if (tutorialStart) {
		if (Timer::now() > subStartTime + 8 && !s_1)
		{
			SubtitleManager::playSubtitle("HUB_01");
			SoundManager::getInstance().PlaySound("intro");
			subStartTime = Timer::now();
			s_1 = true;

		}
		else  if (Timer::now() > subStartTime + 10 && !s_2)

		{
			SubtitleManager::playSubtitle("Tutorial_01",sf::Color::Yellow);
			subStartTime = Timer::now();
			s_2 = true;
		}
		else  if (Timer::now() > subStartTime + 10 && !s_6)

		{
			SubtitleManager::playSubtitle("Tutorial_02",sf::Color::Yellow);
			subStartTime = Timer::now();
			s_6 = true;
		}

		else  if (Timer::now() > subStartTime + 10 && !s_3)

		{
			SubtitleManager::playSubtitle("HUB_02",sf::Color::White);
			SoundManager::getInstance().PlaySound("searchroom");
			subStartTime = Timer::now();
			s_3 = true;
		}
		else  if (Timer::now() > subStartTime + 10 && !s_4)

		{
			SubtitleManager::playSubtitle("HUB_03");
			subStartTime = Timer::now();
			s_4 = true;
		}
		else  if (Timer::now() > subStartTime + 10 && !s_5)

		{
			SubtitleManager::playSubtitle("Tutorial_03",sf::Color::Yellow);
			subStartTime = Timer::now();
			s_5 = true;
		}


	}
}

void LevelManager::setWorldWindow(PhysicsWorld * pWorld, sf::Window * pWindow, DebugHud * hud)
{
	_world = pWorld;
	_window = pWindow;
	_hud = hud;
}

