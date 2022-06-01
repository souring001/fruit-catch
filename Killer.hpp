#ifndef Killer_hpp
#define Killer_hpp

#include "cocos2d.h"

class Killer : public cocos2d::Sprite {
 protected:
  bool init() override;
 public:
  void update(float dt) override;
  CREATE_FUNC(Killer);
};

#endif /* Killer_hpp */
