#include "Chest.h"

#include "cocos2d.h"

#include "Constants.h"
#include "CategoryBits.h"
#include "map/GameMapManager.h"
#include "util/box2d/b2BodyBuilder.h"

using cocos2d::Sprite;
using vigilante::kPpm;
using vigilante::category_bits::kInteractableObject;
using vigilante::category_bits::kGround;
using vigilante::category_bits::kPlatform;
using vigilante::category_bits::kWall;
using vigilante::category_bits::kFeet;

namespace vigilante {

Chest::Chest()
    : DynamicActor(1, 2), _isOpened() {
  _bodySprite = Sprite::create("Texture/interactable_object/chest/chest_close.png");
  _bodySprite->getTexture()->setAliasTexParameters();
}


void Chest::showOnMap(float x, float y) {
  if (!_isShownOnMap) {
    short categoryBits = kInteractableObject;
    short maskBits = kGround | kPlatform | kWall;
    defineBody(b2BodyType::b2_dynamicBody, categoryBits, maskBits, x, y);

    _bodySprite = Sprite::create("Texture/interactable_object/chest/chest_close.png");
    _bodySprite->getTexture()->setAliasTexParameters();

    GameMapManager::getInstance()->getLayer()->addChild(_bodySprite, 33);
    _isShownOnMap = true;
  }
}

void Chest::defineBody(b2BodyType bodyType, short categoryBits, short maskBits, float x, float y) {
  b2BodyBuilder bodyBuilder(GameMapManager::getInstance()->getWorld());

  _body = bodyBuilder.type(bodyType)
    .position(x, y, kPpm)
    .buildBody();

  bodyBuilder.newRectangleFixture(16 / 2, 16 / 2, kPpm)
    .categoryBits(categoryBits)
    .maskBits(maskBits | kFeet)
    .setSensor(true)
    .setUserData(this)
    .buildFixture();

  bodyBuilder.newRectangleFixture(16 / 2, 16 / 2, kPpm)
    .categoryBits(categoryBits)
    .maskBits(maskBits)
    .setUserData(this)
    .buildFixture();
}


void Chest::onInteract(Character* user) {
  cocos2d::log("interacting with chest!");
}

bool Chest::willInteractOnContact() const {
  return false;
}

} // namespace 
