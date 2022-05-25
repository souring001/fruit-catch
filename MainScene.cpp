#include "MainScene.hpp"

USING_NS_CC;

MainScene::MainScene() : _player(NULL) {
}

MainScene::~MainScene() {
  CC_SAFE_RELEASE_NULL(_player);
}

Scene* MainScene::createScene() {
  auto scene = Scene::create();
  auto layer = MainScene::create();
  scene->addChild(layer);
  return scene;
}

bool MainScene::init() {
  if (!Layer::init()) {
    return false;
  }
  
  auto director = Director::getInstance();
  auto size = director->getWinSize();
  auto background = Sprite::create("background.png");
  background->setPosition(Vec2(size.width / 2.0, size.height / 2.0));
  this->addChild(background);
  
  this->setPlayer(Sprite::create("player.png"));
  _player->setPosition(Vec2(size.width / 2.0, size.height - 445));
  this->addChild(_player);
  
  auto listener = EventListenerTouchOneByOne::create(); // 画面がタッチされたことを取得するイベントリスナー
  listener->onTouchBegan = [](Touch* touch, Event* event) {
    log("Touch at (%f, %f)", touch->getLocation().x, touch->getLocation().y);
    return true;
  };
  listener->onTouchMoved = [this](Touch* touch, Event* event) {
    Vec2 delta = touch->getDelta();
    Vec2 position = _player->getPosition();
    Vec2 newPosition = position + delta;
    
    auto winSize = Director::getInstance()->getWinSize();
    newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));
    
    _player->setPosition(newPosition);
  };
  director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
  
  return true;
}
