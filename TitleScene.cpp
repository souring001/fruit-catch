#include "TitleScene.hpp"

#include "MainScene.hpp"
#include "AudioEngine.h"

USING_NS_CC;

Scene* TitleScene::createScene() {
  const auto scene = Scene::create();
  const auto layer = TitleScene::create();
  scene->addChild(layer);
  return scene;
}

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

bool TitleScene::init() {
  if(!Layer::init()) {
    return false;
  }
  
  const auto director = Director::getInstance();
  const auto winSize = director->getWinSize();
  
  const auto background = Sprite::create("title_background.png");
  background->setPosition(winSize.width / 2.0, winSize.height / 2.0);
  this->addChild(background);
  
  const auto logo = Sprite::create("title_logo.png");
  logo->setPosition(winSize.width / 2.0, winSize.height - 150);
  this->addChild(logo);
  
  return true;
}

void TitleScene::onEnterTransitionDidFinish() {
  AudioEngine::play2d("title.caf", true);
}





