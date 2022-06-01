#include "Apple.hpp"

USING_NS_CC;

bool Apple::init() {
  if (!Sprite::initWithFile("fruit0.png")) {
    return false;
  }
  
  auto physicsBody = PhysicsBody::createCircle(this->getContentSize().width / 2.0);
  physicsBody->setGravityEnable(false);
  physicsBody->setCategoryBitmask(0x01);
  physicsBody->setCollisionBitmask(0x00);
  this->setPhysicsBody(physicsBody);
  
  this->scheduleUpdate();
  
  return true;
}

void Apple::update(float dt) {
}
