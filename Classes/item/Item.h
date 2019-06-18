#ifndef VIGILANTE_ITEM_H_
#define VIGILANTE_ITEM_H_

#include <string>

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include "DynamicActor.h"
#include "Importable.h"

namespace vigilante {

class Item : public DynamicActor, public Importable {
 public:
  enum Type {
    EQUIPMENT,
    CONSUMABLE,
    MISC,
    SIZE
  };

  struct Profile {
    Profile(const std::string& jsonFileName);
    virtual ~Profile() = default;

    Item::Type itemType;
    std::string textureResDir;
    std::string name;
    std::string desc;
  };

  virtual ~Item() = default;
  virtual void showOnMap(float x, float y) override; // DynamicActor
  virtual void import(const std::string& jsonFileName) override; // Importable

  Item::Profile& getItemProfile();
  std::string getIconPath() const;

 protected:
  Item(const std::string& jsonFileName);
  void defineBody(b2BodyType bodyType, short categoryBits, short maskBits, float x, float y);

  static const int _kNumAnimations;
  static const int _kNumFixtures;

  Item::Profile _itemProfile;
};

} // namespace vigilante

#endif // VIGILANTE_ITEM_H_
