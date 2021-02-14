// Copyright (c) 2018-2021 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_CONTROL_HINTS_H_
#define VIGILANTE_CONTROL_HINTS_H_

#include <array>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>

#include <cocos2d.h>
#include <2d/CCLabel.h>
#include <ui/UILayout.h>
#include <ui/UIImageView.h>
#include "ui/Colorscheme.h"

namespace vigilante {

// ControlHints contains a fixed number of "profiles"
// for the game to switch between.
//
// Each profile may contains up to 3 ControlHints::Hint,
// where a ControlHints::Hint consists of an icon and a label.

class ControlHints {
 public:
  static ControlHints* getInstance();
  virtual ~ControlHints() = default;

  enum Profile {
    GAME,
    PAUSE_MENU,
    SIZE
  };

  ControlHints::Profile getCurrentProfile() const;
  void pushProfile(ControlHints::Profile profile);
  void popProfile();


  bool isShown(const cocos2d::EventKeyboard::KeyCode keyCode);

  void insert(const cocos2d::EventKeyboard::KeyCode keyCode,
              const std::string& text,
              const cocos2d::Color4B& textColor=colorscheme::kWhite);

  void remove(const cocos2d::EventKeyboard::KeyCode keyCode);


  bool isVisible() const;
  void setVisible(bool visible);
  cocos2d::Layer* getLayer() const;

 private:
  class Hint final {
   public:
    Hint(const cocos2d::EventKeyboard::KeyCode keyCode,
         const std::string& text,
         const cocos2d::Color4B& textColor);
    ~Hint() = default;

    cocos2d::Size getContentSize() const;
    cocos2d::ui::Layout* getLayout() const;
    cocos2d::EventKeyboard::KeyCode getKeyCode() const;

   private:
    static const int _kIconLabelGap;

    cocos2d::ui::Layout* _layout;
    cocos2d::ui::ImageView* _icon;
    cocos2d::Label* _label;
    cocos2d::EventKeyboard::KeyCode _keyCode;
  };

  ControlHints();
  void normalize();
  void showAll();
  void hideAll();
  std::vector<ControlHints::Hint>& getCurrentProfileHints();

  static const int _kHintGap;
  cocos2d::Layer* _layer;

  std::array<std::vector<ControlHints::Hint>, ControlHints::Profile::SIZE> _profiles;
  std::stack<ControlHints::Profile> _currentProfileStack;  
};

}  // namespace vigilante

#endif  // VIGILANTE_CONTROL_HINTS_H_
