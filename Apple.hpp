#ifndef Apple_hpp
#define Apple_hpp

#include "Killer.hpp"
#include "cocos2d.h"

class Apple : public Killer {
 protected:
  bool init() override;
 public:
  void update(float dt) override;
  CREATE_FUNC(Apple);
};

#endif /* Apple_hpp */
