#ifndef MainScene_hpp
#define MainScene_hpp

#include "Killer.hpp"
#include "Player.hpp"
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
  
  static std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> _keys;

  Killer* addFruit();
  void addApple(cocos2d::Vec2 pos, cocos2d::Vec2 vel, float delay);
  bool removeKiller(Killer *killer);
  void hitKiller(Killer *killer);
  void onResult();
  void onHit();
  void setFont(cocos2d::Label *label, float x, float y) const;
  void initBG(float x, float y);
  void initPlayer(float x, float y);
  void addTouchListener();
  void initLabel();
  void addReadyLabel();
  float generateRandom(float min, float max);
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
  bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode);

 protected:
  MainScene();
  virtual ~MainScene();
  bool init() override;
  
 public:
  static cocos2d::Scene* createScene();
  void update(float dt);
  void onEnterTransitionDidFinish() override;
  
  CREATE_FUNC(MainScene);
  CC_SYNTHESIZE(cocos2d::Vector<Killer *>, _killers, Killers);
  CC_SYNTHESIZE(int, _hp, Hp);
  CC_SYNTHESIZE(float, _timer, Timer);
  CC_SYNTHESIZE(bool, _isHit, IsHit);
  CC_SYNTHESIZE(bool, _isDead, IsDead);
  CC_SYNTHESIZE(GameState, _state, State);
  CC_SYNTHESIZE(std::mt19937, _engine, Engine);
  CC_SYNTHESIZE_RETAIN(Player *, _player, Player); // set時に既にあるものをrelease()する
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _hpLabel, HpLabel);
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _timerLabel, TimerLabel);
  CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Label *>, _labels, Labels);
 };

#endif /* MainScene_hpp */
