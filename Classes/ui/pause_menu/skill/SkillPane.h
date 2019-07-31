// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_SKILL_PANE_H_
#define VIGILANTE_SKILL_PANE_H_

#include <vector>
#include <string>
#include <memory>

#include <cocos2d.h>
#include <2d/CCLabel.h>
#include <ui/UILayout.h>
#include <ui/UIImageView.h>
#include "ui/pause_menu/AbstractPane.h"
#include "ui/pause_menu/skill/SkillListView.h"

namespace vigilante {

class SkillPane : public AbstractPane {
 public:
  explicit SkillPane(PauseMenu* pauseMenu);
  virtual ~SkillPane() = default;

  virtual void update() override;
  virtual void handleInput() override;

 private:
  cocos2d::ui::ImageView* _background;
  std::unique_ptr<SkillListView> _skillListView;
};

} // namespace vigilante

#endif // VIGILANTE_SKILL_PANE_H_
