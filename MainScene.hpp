#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"

class MainScene : public cocos2d::Layer {
 protected:
  MainScene();
  virtual ~MainScene();
  bool init() override;
  
 public:
  static cocos2d::Scene* createScene();
  void update(float dt);
  
  enum class GameState {
    PLAYING,
    RESULT,
  };
  
  CREATE_FUNC(MainScene);
  CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);
  CC_SYNTHESIZE(int, _hp, Hp);
  CC_SYNTHESIZE(float, _timer, Timer);
  CC_SYNTHESIZE(GameState, _state, State);
  CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player); // set時に既にあるものをrelease()する
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _hpLabel, HpLabel);
  CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _timerLabel, TimerLabel);

 private:
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
  void setFont(cocos2d::Label *label, float x, float y) const;
  void initBG(float x, float y);
  void initPlayer(float x, float y);
  void addTouchListener();
  void initLabel();
};

#endif /* MainScene_hpp */
