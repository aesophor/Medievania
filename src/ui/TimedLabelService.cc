// Copyright (c) 2018-2021 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "TimedLabelService.h"

#include "AssetManager.h"

using std::string;
using cocos2d::Layer;
using cocos2d::Label;
using cocos2d::MoveBy;
using cocos2d::FadeOut;
using cocos2d::CallFunc;
using cocos2d::Sequence;
using cocos2d::CameraFlag;
using vigilante::asset_manager::kRegularFont;
using vigilante::asset_manager::kRegularFontSize;

namespace vigilante {

const float TimedLabelService::_kDeltaX = 0.0f;
const float TimedLabelService::_kDeltaY = 13.0f;
const float TimedLabelService::_kMoveUpDuration = .2f;
const float TimedLabelService::_kFadeDuration = 1.0f;

TimedLabelService::TimedLabelService(int startingX, int startingY,
                                     uint8_t maxLabelCount, uint8_t labelLifetime,
                                     TimedLabelService::TimedLabel::Alignment alignment)
    : _layer(Layer::create()),
      _kStartingX(startingX),
      _kStartingY(startingY),
      _kMaxLabelCount(maxLabelCount),
      _kLabelLifetime(labelLifetime),
      _kAlignment(alignment) {}


void TimedLabelService::update(float delta) {
  for (auto& notification : _labelQueue) {
    notification.timer += delta;

    if (notification.timer >= notification.lifetime) {
      notification.label->runAction(Sequence::createWithTwoActions(
        FadeOut::create(_kFadeDuration),
        CallFunc::create([=]() {
          // After the label fully fades out, remove the label from _layer.
          _layer->removeChild(notification.label);
        })
      ));
      // Also remove the notification object from _labelQueue.
      auto& q = _labelQueue;
      q.erase(std::remove(q.begin(), q.end(), notification), q.end());
    }
  }
}

void TimedLabelService::show(const string& message) {
  // If the number of notifications being displayed has surpassed _kMaxLabelCount,
  // then remove the earliest notification.
  if ((int) _labelQueue.size() > _kMaxLabelCount) {
    _layer->removeChild(_labelQueue.front().label);
    _labelQueue.pop_front();
  }

  // Move previous notifications up.
  for (auto label : _labelQueue) {
    label.label->runAction(MoveBy::create(_kMoveUpDuration, {_kDeltaX, _kDeltaY}));
  }

  // Display the new notification.
  // Note that cocos2d::Layer::setCameraMask() can only apply the given mask to
  // the children that are in the _layer at that moment. If we create a label/sprite
  // afterwards, the mask is the default one, even if we add it as a child of that layer.
  TimedLabel timedLabel(message, _kLabelLifetime, _kAlignment);
  timedLabel.label->setPosition(_kStartingX, _kStartingY);
  timedLabel.label->runAction(MoveBy::create(_kMoveUpDuration, {_kDeltaX, _kDeltaY}));
  _labelQueue.push_back(timedLabel);

  _layer->addChild(timedLabel.label);
  _layer->setCameraMask(_layer->getCameraMask());
}

Layer* TimedLabelService::getLayer() const {
  return _layer;
}


const TimedLabelService::TimedLabel::Alignment TimedLabelService::TimedLabel::kLeft = {0, 1};
const TimedLabelService::TimedLabel::Alignment TimedLabelService::TimedLabel::kCenter = {0.5, 1};
const TimedLabelService::TimedLabel::Alignment TimedLabelService::TimedLabel::kRight = {1, 1};

TimedLabelService::TimedLabel::TimedLabel(const string& text, float lifetime,
                                          TimedLabel::Alignment alignment)
    : label(Label::createWithTTF(text, kRegularFont, kRegularFontSize)),
      lifetime(lifetime),
      timer() {
  label->setAnchorPoint(alignment);
  label->getFontAtlas()->setAliasTexParameters();
}

bool TimedLabelService::TimedLabel::operator==(const TimedLabel& other) {
  return this->label == other.label;
}

}  // namespace vigilante
