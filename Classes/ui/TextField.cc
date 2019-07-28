// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "TextField.h"

#include "AssetManager.h"
#include "input/InputManager.h"
#include "ui/console/Console.h"
#include "util/KeyCodeUtil.h"

#define CURSOR_CHAR "|"
#define CURSOR_BLINK_INTERVAL 0.7f

using cocos2d::Label;
using cocos2d::ui::Layout;
using cocos2d::Event;
using cocos2d::EventKeyboard;
using vigilante::asset_manager::kRegularFont;
using vigilante::asset_manager::kRegularFontSize;

namespace vigilante {

TextField::TextField()
    : _layout(Layout::create()),
      _label(Label::createWithTTF(CURSOR_CHAR, kRegularFont, kRegularFontSize)),
      _buffer(),
      _timer(),
      _isRecevingInput(),
      _isCursorVisible() {
  _label->setAnchorPoint({0, 0});
  _layout->addChild(_label);
}


void TextField::update(float delta) {
  _timer += delta;

  if (_timer >= CURSOR_BLINK_INTERVAL) {
    _timer = 0;
    toggleCursor();
  }
}

void TextField::handleInput() {
  // FIXME: clean up this shit god dammit
  auto onKeyPressedEvLstnr = [=](EventKeyboard::KeyCode keyCode, Event*) {
    if (keyCode == EventKeyboard::KeyCode::KEY_GRAVE) {
      _isRecevingInput = false;
      InputManager::getInstance()->popEvLstnr();
      Console::getInstance()->getLayer()->setVisible(false);
      return;
    }

    if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
      Console::getInstance()->executeCmd(_buffer);
      clear();
      return;
    }

    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE && !_buffer.empty()) {
      _buffer.pop_back();
      _label->setString(_buffer + CURSOR_CHAR);
      return;
    }

    char c = keycode_util::keyCodeToAscii(keyCode);
    if (c != 0x00) {
      _buffer += c;
      _label->setString(_buffer + CURSOR_CHAR);
    }
  };

  if (!_isRecevingInput) {
    _isRecevingInput = true;
    InputManager::getInstance()->pushEvLstnr(onKeyPressedEvLstnr);
  }
}

void TextField::clear() {
  _buffer.clear();
  _label->setString(CURSOR_CHAR);
}

void TextField::toggleCursor() {
  _label->setString((_isCursorVisible) ? _buffer : _buffer + CURSOR_CHAR);
  _isCursorVisible = !_isCursorVisible;
}


Layout* TextField::getLayout() const {
  return _layout;
}

} // namespace vigilante
