// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "MainMenuScene.h"

#include "AssetManager.h"
#include "MainGameScene.h"
#include "ui/Colorscheme.h"

using std::array;
using std::string;
using std::unique_ptr;
using cocos2d::Director;
using cocos2d::Scene;
using cocos2d::Label;
using cocos2d::ui::ImageView;
using cocos2d::EventKeyboard;
using vigilante::asset_manager::kBoldFont;
using vigilante::asset_manager::kRegularFontSize;

namespace vigilante {

const array<string, MainMenuScene::Option::SIZE> MainMenuScene::_kOptionStr = {{
  "New Game",
  "Load Game",
  "Options",
  "Exit"
}};

const string MainMenuScene::_kCopyrightStr = "© 2019 Aesophor Softworks";
const string MainMenuScene::_kVersionStr = "0.0.1 Alpha";

const int MainMenuScene::_kMenuOptionGap = 20;
const int MainMenuScene::_kFooterLabelPadding = 10;


bool MainMenuScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto winSize = Director::getInstance()->getWinSize();

  // Initialize background ImageView.
  _background = ImageView::create(vigilante::asset_manager::kMainMenuBg);
  _background->setAnchorPoint({0, 0});
  addChild(_background);

  // Initialize labels.
  for (int i = 0; i < static_cast<int>(Option::SIZE); i++) {
    Label* label = Label::createWithTTF(_kOptionStr[i], kBoldFont, kRegularFontSize);
    label->getFontAtlas()->setAliasTexParameters();
    label->setPosition(winSize.width / 2, winSize.height / 2 - _kMenuOptionGap * (i + 1));
    addChild(label);
    _labels.push_back(label);
  }
  _current = 0;
  _labels[_current]->setTextColor(vigilante::colorscheme::kRed);

  // Initialize footer labels.
  Label* copyrightLabel = Label::createWithTTF(_kCopyrightStr, kBoldFont, kRegularFontSize);
  copyrightLabel->setAnchorPoint({0.5, 0});
  copyrightLabel->setPosition(winSize.width / 2, copyrightLabel->getContentSize().height + _kFooterLabelPadding);
  addChild(copyrightLabel);

  Label* versionLabel = Label::createWithTTF(_kVersionStr, kBoldFont, kRegularFontSize);
  versionLabel->setAnchorPoint({1, 0});
  versionLabel->setPosition(winSize.width - _kFooterLabelPadding, versionLabel->getContentSize().height + _kFooterLabelPadding);
  addChild(versionLabel);

  // Initialize InputManager.
  _inputMgr = InputManager::getInstance();
  _inputMgr->activate(this);

  schedule(schedule_selector(MainMenuScene::update));
  return true;
}

void MainMenuScene::update(float) {
  handleInput();
}

void MainMenuScene::handleInput() {
  if (_inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
    if (_current == 0) {
      return;
    }
    _labels[_current--]->setTextColor(vigilante::colorscheme::kWhite);
    _labels[_current]->setTextColor(vigilante::colorscheme::kRed);
  } else if (_inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
    if (_current == static_cast<int>(Option::SIZE) - 1) {
      return;
    }
    _labels[_current++]->setTextColor(vigilante::colorscheme::kWhite);
    _labels[_current]->setTextColor(vigilante::colorscheme::kRed);
  } else if (_inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_ENTER)) {
    switch (static_cast<Option>(_current)) {
      case Option::NEW_GAME: {
        _inputMgr->deactivate();
        Scene* scene = MainGameScene::create();
        Director::getInstance()->pushScene(scene);
        break;
      }
      case Option::LOAD_GAME:
        break;
      case Option::OPTIONS:
        break;
      case Option::EXIT:
        Director::getInstance()->end();
        break;
      default:
        break;
    }
  }
}

} // namespace vigilante
