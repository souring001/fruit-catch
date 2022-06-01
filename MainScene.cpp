#include "MainScene.hpp"

#include "TitleScene.hpp"
#include "AudioEngine.h"

USING_NS_CC;

const int FRUIT_TOP_MARGINE = 40;
const int MAX_HP = 2;

MainScene::MainScene()
: _hp(MAX_HP)
, _timer(0.f)
, _isHit(false)
, _isDead(false)
, _state(GameState::READY)
, _player(NULL)
, _hpLabel(NULL)
, _timerLabel(NULL) {
  std::random_device rdev;
  _engine.seed(rdev());
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
    float p = 0.02 * (1 + powf(1.05f, _timer));
    p = MIN(p, 0.5);
    log("p: %f", p);
    
    const float random = this->generateRandom(0, 1);
    if (random < p) {
      this->addFruit();
    }
    
    for (const auto& fruit : _fruits) {
      Vec2 busketPosition = _player->getPosition();
      Rect boundingBox = fruit->getBoundingBox();
      bool isHit = boundingBox.containsPoint(busketPosition);
      if (isHit && !_isHit) {
        this->hitFruit(fruit);
      }
    }
    
    _timer += dt;
    int time = static_cast<int>(_timer);
    _timerLabel->setString(StringUtils::toString(time));
    
    if (_hp <= 0) {
      _state = GameState::ENDING;
      
      const auto finish = Sprite::create("finish.png");
      const auto winSize = Director::getInstance()->getWinSize();
      
      finish->setPosition(winSize.width / 2.0, winSize.height / 2.0);
      finish->setScale(0);
      
      AudioEngine::pauseAll();
      AudioEngine::play2d("finish.caf");
      
      const auto appear = EaseExponentialIn::create(ScaleTo::create(0.25, 1.0));
      const auto disappear = EaseExponentialIn::create(ScaleTo::create(0.25, 0));
      
      finish->runAction(Sequence::create(appear,
                                         DelayTime::create(2.0),
                                         disappear,
                                         DelayTime::create(1.0),
                                         CallFunc::create([this] {
        _state = GameState::RESULT;
        this->onResult();
      }),
                                         NULL));
      this->addChild(finish);
      
      // GAME OVER 演出のため非表示にする
      for (const auto& label : _labels) {
        label->setVisible(false);
      }
      for (const auto& fruit : _fruits) {
        fruit->setVisible(false);
      }
      _isDead = true;
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
    const auto scene = TitleScene::createScene();
    const auto transition = TransitionFade::create(1.0, scene);
    Director::getInstance()->replaceScene(transition);
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
  
  this->onHit();
}

void MainScene::onHit() {
  _isHit = true;
  
  Vector<SpriteFrame *> frames;
  const auto playerSize = _player->getContentSize();
  const int animationFrameCount = 2;
  for (int i = 0; i < animationFrameCount; i++) {
    const auto rect = Rect(playerSize.width * i, 0, playerSize.width, playerSize.height);
    const auto frame = SpriteFrame::create("player_crash.png", rect);
    frames.pushBack(frame);
  }
  
  const auto animation = Animation::createWithSpriteFrames(frames, 10.0 / 60.0);
  animation->setLoops(5);
  animation->setRestoreOriginalFrame(true);
  _player->runAction(Sequence::create(Animate::create(animation),
                                      CallFunc::create([this] {
    _isHit = false;
  }),
                                      NULL));
  
  AudioEngine::play2d("catch_fruit.caf");
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
  _player->setScale(0.5);
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
    if (!_isDead) {
      Vec2 delta = touch->getDelta();
      Vec2 position = _player->getPosition();
      Vec2 newPosition = position + delta;
      
      const auto winSize = Director::getInstance()->getWinSize();
      newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));
      
      _player->setPosition(newPosition);
    }
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
  _labels.pushBack(hpLabel);
  
  const auto hpLabelHeader = Label::createWithTTF("HP", "4x4kanafont.ttf", 16);
  this->setFont(hpLabelHeader, winSize.width / 2.0 * 1.2, winSize.height - 20);
  this->addChild(hpLabelHeader);
  _labels.pushBack(hpLabelHeader);
  
  int time = static_cast<int>(_timer);
  const auto timerLabel = Label::createWithTTF(StringUtils::toString(time), "4x4kanafont.ttf", 16);
  this->setTimerLabel(timerLabel);
  this->setFont(timerLabel, winSize.width / 2.0 * 0.8, winSize.height - 20);
  this->addChild(timerLabel);
  _labels.pushBack(timerLabel);
  
  const auto timerLabelHeader = Label::createWithTTF("TIME", "4x4kanafont.ttf", 16);
  this->setFont(timerLabelHeader, winSize.width / 2.0 * 0.4, winSize.height - 20);
  this->addChild(timerLabelHeader);
  _labels.pushBack(timerLabelHeader);
}

void MainScene::onEnterTransitionDidFinish() {
  Layer::onEnterTransitionDidFinish();
  // BGM再生
  AudioEngine::play2d("main.caf", true);
  this->addReadyLabel();
}

void MainScene::addReadyLabel() {
  const auto winSize = Director::getInstance()->getWinSize();
  const auto center = Vec2(winSize.width / 2.0, winSize.height / 2.0);
  
  const auto ready = Sprite::create("ready.png");
  ready->setScale(0);
  ready->setPosition(center);
  this->addChild(ready);
  
  ready->runAction(Sequence::create(ScaleTo::create(0.25, 1),
                                    DelayTime::create(1.0),
                                    CallFunc::create([this, center] {
    const auto start = Sprite::create("start.png");
    start->setPosition(center);
    start->runAction(Sequence::create(Spawn::create(EaseIn::create(ScaleTo::create(0.5, 5.0), 0.5),
                                                    FadeOut::create(0.5),
                                                    NULL),
                                      RemoveSelf::create(),
                                      NULL));
    this->addChild(start);
    _state = GameState::PLAYING;
    AudioEngine::play2d("stat.caf");
  }),
                                    RemoveSelf::create(),
                                    NULL));
}

float MainScene::generateRandom(float min, float max) {
  std::uniform_real_distribution<float> dest(min, max);
  return dest(_engine);
}
