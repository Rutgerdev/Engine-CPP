#include "PressurePlateBehaviour.h"

#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include "mge/core/collision/RigidbodyGameObject.h"
#include "mge/core/collision/StaticGameObject.h"
#include "mge/core/collision/PhysicsWorld.h"
#include <SFML/Graphics.hpp>

#include "mge/core/collision/BoxTrigger.h"
#include "mge/core/collision/Collision.h"

#include "mge/core/Mesh.hpp"
#include "mge/config.hpp"

#include "mge/materials/BasicTextureLight.hpp"

PressurePlateBehaviour::PressurePlateBehaviour(PhysicsWorld * world)
{	
	hit = false;
	_world = world;
}

PressurePlateBehaviour::~PressurePlateBehaviour()
{
}

void PressurePlateBehaviour::update(float pStep) {
}

void PressurePlateBehaviour::SetStatue(GameObject * pStatue) {
	_statue = pStatue;
}

void PressurePlateBehaviour::OnCollision(Collision collision)
{
	if (collision.getHitBy() == _statue->getName() && !hit)
	{
		_activated = true;
		hit = true;
		neRigidBody * rigidbody = dynamic_cast<RigidbodyGameObject*>(_statue)->GetRigidBody();
		dynamic_cast<RigidbodyGameObject*>(_statue)->deleteObject();

		StaticGameObject * obj = new StaticGameObject("collider", _owner->getLocalPosition() + glm::vec3(0,3,0) ,_world);
		obj->setMesh(Mesh::load(config::MGE_MODEL_PATH + "statue.obj"));
		obj->setMaterial(new BasicTextureLit(Texture::load(config::MGE_TEXTURE_PATH + "statue_DIFF (TEMP).png"), 0.1f));

		_world->add(obj);
	}
}

