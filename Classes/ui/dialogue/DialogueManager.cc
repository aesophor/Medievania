// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "DialogueManager.h"

using std::unique_ptr;

namespace vigilante {

DialogueManager* DialogueManager::_instance = nullptr;

DialogueManager* DialogueManager::getInstance() {
  if (!_instance) {
    _instance = new DialogueManager();
  }
  return _instance;
}

DialogueManager::DialogueManager()
    : _subtitles(new Subtitles()),
      _dialogueMenu(new DialogueMenu()) {}


void DialogueManager::update(float delta) {
  _subtitles->update(delta);
}

void DialogueManager::handleInput() {
  if (_dialogueMenu->getLayer()->isVisible()) {
    _dialogueMenu->handleInput();
  } else if (_subtitles->getLayer()->isVisible()) {
    _subtitles->handleInput();
  }
}


Npc* DialogueManager::getTargetNpc() const {
  return _targetNpc;
}

Subtitles* DialogueManager::getSubtitles() const {
  return _subtitles.get();
}

DialogueMenu* DialogueManager::getDialogueMenu() const {
  return _dialogueMenu.get();
}

Dialogue* DialogueManager::getCurrentDialogue() const {
  return (_targetNpc) ? _targetNpc->getDialogueTree().getCurrentNode() : nullptr;
}


void DialogueManager::setTargetNpc(Npc* npc) {
  _targetNpc = npc;
}

void DialogueManager::setCurrentDialogue(Dialogue* dialogue) const {
  if (!_targetNpc) {
    return;
  }
  _targetNpc->getDialogueTree().setCurrentNode(dialogue);
}

} // namespace vigilante