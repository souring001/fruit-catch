#include "MainScene.hpp"

USING_NS_CC;

const int FRUIT_TOP_MARGINE = 40;
const int FRUIT_SPAWN_RATE = 20;
const int MAX_HP = 2;

MainScene::MainScene()
: _hp(MAX_HP)
, _timer(0.f)
, _state(GameState::PLAYING)
, _player(NULL)
, _hpLabel(NULL)
, _timerLabel(NULL) {
}

MainScene::~MainScene() {
  CC_SAFE_RELEASE_NULL(_player);
  CC_SAFE_RELEASE_NULL(_hpLabel);
  CC_SAFE_RELEASE_NULL(_timerLabel);
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
  
  this->scheduleUpdate();
  
  const auto hpString = StringUtils::toString(_hp) + " / " + StringUtils::toString(MAX_HP);
  auto hpLabel = Label::createWithTTF(hpString, "4x4kanafont.ttf", 16);
  this->setHpLabel(hpLabel);
  this->setFont(hpLabel, size.width / 2.0 * 1.7, size.height - 20);
  this->addChild(_hpLabel);
  
  auto hpLabelHeader = Label::createWithTTF("HP", "4x4kanafont.ttf", 16);
  this->setFont(hpLabelHeader, size.width / 2.0 * 1.2, size.height - 20);
  this->addChild(hpLabelHeader);
  
  int time = static_cast<int>(_timer);
  auto timerLabel = Label::createWithTTF(StringUtils::toString(time), "4x4kanafont.ttf", 16);
  this->setTimerLabel(timerLabel);
  this->setFont(timerLabel, size.width / 2.0 * 0.8, size.height - 20);
  this->addChild(timerLabel);
  
  auto timerLabelHeader = Label::createWithTTF("TIME", "4x4kanafont.ttf", 16);
  this->setFont(timerLabelHeader, size.width / 2.0 * 0.4, size.height - 20);
  this->addChild(timerLabelHeader);
  
  return true;
}

Sprite* MainScene::addFruit() {
  auto winSize = Director::getInstance()->getWinSize();
  int fruitType = rand() % static_cast<int>(FruitType::COUNT);
  
  std::string filename = StringUtils::format("fruit%d.png", fruitType);
  auto fruit = Sprite::create(filename);
  fruit->setTag(fruitType);
  
  auto fruitSize = fruit->getContentSize();
  float fruitXPos = rand() % static_cast<int>(winSize.width);
  
  fruit->setPosition(Vec2(fruitXPos, winSize.height - FRUIT_TOP_MARGINE - fruitSize.height / 2.0));
  
  this->addChild(fruit);
  _fruits.pushBack(fruit);
  
  auto ground = Vec2(fruitXPos, 0);
  auto fall = MoveTo::create(3, ground);
  
  auto remove = CallFuncN::create([this](Node *node) {
    auto sprite = dynamic_cast<Sprite *>(node);
    this->removeFruit(sprite);
  });
  
  auto sequence = Sequence::create(fall, remove, NULL);
  fruit->runAction(sequence);
  return fruit;
}

bool MainScene::removeFruit(cocos2d::Sprite *fruit) {
  if (_fruits.contains(fruit)) {
    fruit->removeFromParent();
    _fruits.eraseObject(fruit);
    return true;
  }
  return false;
}

void MainScene::update(float dt) {
  if (_state == GameState::PLAYING) {
    int random = rand() % FRUIT_SPAWN_RATE;
    if (random == 0) {
      this->addFruit();
    }
    
    for (const auto& fruit : _fruits) {
      Vec2 busketPosition = _player->getPosition();
      Rect boundingBox = fruit->getBoundingBox();
      bool isHit = boundingBox.containsPoint(busketPosition);
      if (isHit) {
        this->hitFruit(fruit);
      }
    }
    
    _timer += dt;
    int time = static_cast<int>(_timer);
    _timerLabel->setString(StringUtils::toString(time));
    
    if (_hp <= 0) {
      _state = GameState::RESULT;
    }
  }
}

void MainScene::hitFruit(cocos2d::Sprite *fruit) {
  this->removeFruit(fruit);
  _hp--;
  const auto hpString = StringUtils::toString(_hp) + " / " + StringUtils::toString(MAX_HP);
  _hpLabel->setString(StringUtils::toString(hpString));
}

void MainScene::setFont(cocos2d::Label *label, float x, float y) {
  label->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
  label->enableOutline(Color4B::BLACK, 1.5);
  label->setPosition(x, y);
}
