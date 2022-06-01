#include "Apple.hpp"

USING_NS_CC;

bool Apple::init() {
  if (!Sprite::initWithFile("fruit0.png")) {
    return false;
  }
  
  this->scheduleUpdate();
  
  return true;
}

void Apple::update(float dt) {
}
