#ifndef Player_hpp
#define Player_hpp

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
 protected:
  bool init() override;
 public:
  void update(float dt) override;
  CREATE_FUNC(Player);
};

#endif /* Player_hpp */
