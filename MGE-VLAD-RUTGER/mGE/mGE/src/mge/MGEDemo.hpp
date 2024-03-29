#ifndef MGEDEMO_H
#define MGEDEMO_H

#include <mge/core/AbstractGame.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#include "mge/xml/XmlReader.h"

class Swapper;
class GameObject;
class DebugHud;

class MGEDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();
        virtual void initialize();

	protected:
        virtual void _initializeScene();
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();
};

#endif // MGEDEMO_H
