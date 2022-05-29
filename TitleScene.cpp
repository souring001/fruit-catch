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
  
  const auto touchToStart = Sprite::create("title_start.png");
  touchToStart->setPosition(winSize.width / 2.0, 90);
  const auto blink = Sequence::create(FadeTo::create(0.5, 127), FadeTo::create(0.5, 255), NULL);
  touchToStart->runAction(RepeatForever::create(blink));
  this->addChild(touchToStart);
  
  const auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = [this](Touch* touch, Event* event) {
    AudioEngine::play2d("decide.caf");
    
    // 何度も押せないように一度押したらアクションを無効化
    this->getEventDispatcher()->removeAllEventListeners();
    
    const auto delay = DelayTime::create(0.5);
    const auto startGame = CallFunc::create([] {
      const auto scene = MainScene::createScene();
      const auto transition = TransitionPageTurn::create(0.5, scene, true);
      Director::getInstance()->replaceScene(transition);
    });
    this->runAction(Sequence::create(delay, startGame, NULL));
    AudioEngine::pauseAll();
    return true;
  };
  
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
  
  return true;
}

void TitleScene::onEnterTransitionDidFinish() {
  AudioEngine::play2d("title.caf", true);
}





