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
  CREATE_FUNC(MainScene);
};

#endif /* MainScene_hpp */
