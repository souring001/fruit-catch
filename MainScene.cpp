#include "MainScene.hpp"
#include <algorithm>

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
  const auto scene = Scene::create();
  const auto layer = MainScene::create();
  scene->addChild(layer);
  return scene;
}

bool MainScene::init() {
  if (!Layer::init()) {
    return false;
  }
  
  const auto winSize = Director::getInstance()->getWinSize();
  
  this->initBG(winSize.width / 2.0, winSize.height / 2.0);
  this->initPlayer(winSize.width / 2.0, winSize.height - 445);
  this->addTouchListener();
  this->initLabel();
  
  this->scheduleUpdate();
  
  return true;
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
      this->onResult();
    }
  }
}

void MainScene::onResult() {
  _state = GameState::RESULT;
  const auto winSize = Director::getInstance()->getWinSize();
  
  const auto replayButton = MenuItemImage::create("replay_button.png",
                                                  "replay_button_pressed.png",
                                                  [](Ref* ref) {
    const auto scene = MainScene::createScene();
    const auto transition = TransitionFade::create(0.5, scene);
    Director::getInstance()->replaceScene(transition);
  });
  
  const auto titleButton = MenuItemImage::create("title_button.png",
                                                 "title_button_pressed.png",
                                                 [](Ref* ref) {
    // do nothing
  });
  
  const auto menu = Menu::create(replayButton, titleButton, NULL);
  menu->alignItemsVerticallyWithPadding(15);
  menu->setPosition(winSize.width / 2.0f, winSize.height / 2.0);
  this->addChild(menu);
  
}

Sprite* MainScene::addFruit() {
  const auto winSize = Director::getInstance()->getWinSize();
  int fruitType = rand() % static_cast<int>(FruitType::COUNT);
  
  std::string filename = StringUtils::format("fruit%d.png", fruitType);
  const auto fruit = Sprite::create(filename);
  fruit->setTag(fruitType);
  
  const auto fruitSize = fruit->getContentSize();
  float fruitXPos = rand() % static_cast<int>(winSize.width);
  
  fruit->setPosition(Vec2(fruitXPos, winSize.height - FRUIT_TOP_MARGINE - fruitSize.height / 2.0));
  
  this->addChild(fruit);
  _fruits.pushBack(fruit);
  
  const auto ground = Vec2(fruitXPos, 0);
  const auto fall = MoveTo::create(3, ground);
  
  const auto remove = CallFuncN::create([this](Node *node) {
    auto sprite = dynamic_cast<Sprite *>(node);
    this->removeFruit(sprite);
  });
  
  const auto sequence = Sequence::create(fall, remove, NULL);
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

void MainScene::hitFruit(cocos2d::Sprite *fruit) {
  this->removeFruit(fruit);
  _hp -= 1;
  _hp = clampf(_hp, 0, MAX_HP);
  const auto hpString = StringUtils::toString(_hp) + " / " + StringUtils::toString(MAX_HP);
  _hpLabel->setString(StringUtils::toString(hpString));
}

void MainScene::setFont(cocos2d::Label *label, float x, float y) const {
  label->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
  label->enableOutline(Color4B::BLACK, 1.5);
  label->setPosition(x, y);
}

void MainScene::initBG(float x, float y) {
  auto background = Sprite::create("background.png");
  background->setPosition(x, y);
  this->addChild(background);
}

void MainScene::initPlayer(float x, float y) {
  this->setPlayer(Sprite::create("player.png"));
  _player->setPosition(x, y);
  this->addChild(_player);
}

void MainScene::addTouchListener() {
  const auto director = Director::getInstance();
  const auto listener = EventListenerTouchOneByOne::create(); // 画面がタッチされたことを取得するイベントリスナー
  listener->onTouchBegan = [](Touch* touch, Event* event) {
    log("Touch at (%f, %f)", touch->getLocation().x, touch->getLocation().y);
    return true;
  };
  listener->onTouchMoved = [this](Touch* touch, Event* event) {
    Vec2 delta = touch->getDelta();
    Vec2 position = _player->getPosition();
    Vec2 newPosition = position + delta;
    
    const auto winSize = Director::getInstance()->getWinSize();
    newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));
    
    _player->setPosition(newPosition);
  };
  director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MainScene::initLabel() {
  const auto winSize = Director::getInstance()->getWinSize();
  
  const auto hpString = StringUtils::toString(_hp) + " / " + StringUtils::toString(MAX_HP);
  const auto hpLabel = Label::createWithTTF(hpString, "4x4kanafont.ttf", 16);
  this->setHpLabel(hpLabel);
  this->setFont(hpLabel, winSize.width / 2.0 * 1.7, winSize.height - 20);
  this->addChild(_hpLabel);
  
  const auto hpLabelHeader = Label::createWithTTF("HP", "4x4kanafont.ttf", 16);
  this->setFont(hpLabelHeader, winSize.width / 2.0 * 1.2, winSize.height - 20);
  this->addChild(hpLabelHeader);
  
  int time = static_cast<int>(_timer);
  const auto timerLabel = Label::createWithTTF(StringUtils::toString(time), "4x4kanafont.ttf", 16);
  this->setTimerLabel(timerLabel);
  this->setFont(timerLabel, winSize.width / 2.0 * 0.8, winSize.height - 20);
  this->addChild(timerLabel);
  
  const auto timerLabelHeader = Label::createWithTTF("TIME", "4x4kanafont.ttf", 16);
  this->setFont(timerLabelHeader, winSize.width / 2.0 * 0.4, winSize.height - 20);
  this->addChild(timerLabelHeader);
}
