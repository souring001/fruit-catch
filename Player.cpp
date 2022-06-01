#include "Player.hpp"

USING_NS_CC;

bool Player::init() {
  if (!Sprite::initWithFile("player.png")) {
    return false;
  }
  
  this->setScale(0.5);
  this->scheduleUpdate();
  
  return true;
}

void Player::update(float dt) {
}
