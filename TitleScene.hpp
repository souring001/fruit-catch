#ifndef TitleScene_hpp
#define TitleScene_hpp

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer {
 protected:
  TitleScene();
  virtual ~TitleScene();
  bool init() override;
  
 public:
  static cocos2d::Scene* createScene();
  void onEnterTransitionDidFinish() override;
  CREATE_FUNC(TitleScene);
};

#endif /* TitleScene_hpp */
