#ifndef ABSTRACTGAME_H
#define ABSTRACTGAME_H

#include <SFML\Graphics.hpp>

#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <string>
using namespace std;

//class World;
class PhysicsWorld;
class Renderer;
class TriggerManager;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * Your main game loop should run:
 *
 *  initialize
 *  run
 *
 * in order.
 */
class AbstractGame
{
    public:

        AbstractGame();
        virtual ~AbstractGame();

        //creates a window, initializes glew, a renderer and a world instance
        virtual void initialize();
        //run the actual process of updating all objects, rendering them and processing events
        virtual void run();
		int drawCalls;
		int triangleCount;
    protected:

        //methods above delegate behaviour to the methods below so that you can override it in a subclass

        //initialize sfml rendering context
        virtual void _initializeWindow();
        //print info about the current driver version etc
        virtual void _printVersionInfo();
        //initialize the extension wrangler
        virtual void _initializeGlew();
        //create our own custom renderer instance
        virtual void _initializeRenderer();
        virtual void _initializePhysiscs();
        //initialize a scene root to which we can attach/add objects
        virtual void _initializeWorld();

        //initialize the actual scene, to be done by a subclass
        virtual void _initializeScene() = 0;

        //call update on all game objects in the display root
        virtual void _update();
		//call update on all game objects in the display root with fixed timestep
		virtual void _fixedUpdate();

        //render all game objects in the display root
        virtual void _render();
        //process any sfml window events (see SystemEventDispatcher/Listener)
        virtual void _processEvents();

		sf::RenderWindow* _window;  //sfml window to render into
		Renderer* _renderer;        //the renderer class to render the world
		PhysicsWorld* _world;              //the root game object that represents our scene
		bool _running;

		//used for debugging.
		double updateTime;
		double renderTime;
};

#endif // ABSTRACTGAME_H
