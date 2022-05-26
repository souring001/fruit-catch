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
  CREATE_FUNC(MainScene);
  CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player);
  CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);

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
};

#endif /* MainScene_hpp */
