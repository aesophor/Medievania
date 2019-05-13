#ifndef VIGILANTE_DUST_H_
#define VIGILANTE_DUST_H_

#include "cocos2d.h"

namespace vigilante {

// FIXME: a dedicated class is probably unnecessary...
class Dust {
 public:
  Dust(cocos2d::Layer* gameMapLayer, float x, float y);
  virtual ~Dust() = default;
};

} // namespace vigilante

#endif // VIGILANTE_DUST_H_