#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include <random>

class MainScene : public cocos2d::Layer {
 private:
  enum class GameState {
    READY,
    PLAYING,
    ENDING,
    RESULT,
  };

  enum class FruitType {
    APPLE,
    GRAPE,
    ORANGE,
    BANANA,
    CHERRY,
    COUNT,
  };

  cocos2d::Sprite* addFruit();
  bool removeFruit(cocos2d::Sprite *fruit);
  void hitFruit(cocos2d::Sprite *fruit);
  void onResult();
  void onHit();
  void setFont(cocos2d::Label *label, float x, float y) const;
  void initBG(float x, float y);
  void initPlayer(float x, float y);
  void addTouchListener();
  void initLabel();
  void addReadyLabel();
  float generateRandom(float min, float max);

 protected:
  MainScene();
  virtual ~MainScene();
  bool init() override;
  
 public:
  static cocos2d::Scene* createScene();
  void update(float dt);
  void onEnterTransitionDidFinish() override;
  
  CREATE_FUNC(MainScene);
  CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);
  CC_SYNTHESIZE(int, _hp, Hp);
  CC_SYNTHESIZE(float, _timer, Timer);
  CC_SYNTHESIZE(bool, _isHit, IsHit);
  CC_SYNTHESIZE(bool, _isDead, IsDead);
  CC_SYNTHESIZE(GameState, _state, State);
  CC_SYNTHESIZE(std::mt19937, _engine, Engine);
  CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player); // set時に既にあるものをrelease()する
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _hpLabel, HpLabel);
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _timerLabel, TimerLabel);
  CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Label *>, _labels, Labels);
 };

#endif /* MainScene_hpp */
