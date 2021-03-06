// Copyright (c) 2018-2021 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_NPC_H_
#define VIGILANTE_NPC_H_

#include <atomic>
#include <string>
#include <vector>
#include <unordered_set>

#include <cocos2d.h>
#include "Character.h"
#include "Interactable.h"
#include "gameplay/DialogueTree.h"

namespace vigilante {

// Npc: Non-player character
// i.e., enemies and allies are all npcs.
class Npc : public Character, public Interactable {
 public:
  // The "disposition" of an Npc means its "friendliness" toward the player.
  enum class Disposition {
    ALLY,
    ENEMY,
    SIZE
  };

  struct Profile final {
    explicit Profile(const std::string& jsonFileName);
    ~Profile() = default;

    struct DroppedItemData {
      int chance;
      int minAmount;
      int maxAmount;
    };
    // <json, {chance, minAmount, maxAmount}>
    std::unordered_map<std::string, DroppedItemData> droppedItems;

    std::string dialogueTreeJsonFile;
    Npc::Disposition disposition;
    bool isRespawnable;
    bool isRecruitable;
    bool isUnsheathed;
    bool isTradable;
    bool shouldSandbox;
  };

  // In addition to Character::FixtureType, the new version defined in Npc.h
  // has the fourth fixture type.
  enum FixtureType {
    BODY,
    FEET,
    WEAPON,
    INTERACTABLE,  // used in player's interaction with Npcs (with FEET fixture)
    FIXTURE_SIZE
  };


  explicit Npc(const std::string& jsonFileName);
  virtual ~Npc() = default;

  virtual bool showOnMap(float x, float y) override;  // Character
  virtual void update(float delta) override;  // Character
  virtual void import(const std::string& jsonFileName) override;  // Character

  virtual void onKilled() override;  // Character

  virtual void inflictDamage(Character* target, int damage) override;  // Character
  virtual void receiveDamage(Character* source, int damage) override;  // Character
  virtual void interact(Interactable* target) override;  // Character

  virtual void onInteract(Character* user) override;  // Interactable
  virtual bool willInteractOnContact() const override;  // Interactable
  virtual void showHintUI() override;  // Interactable
  virtual void hideHintUI() override;  // Interactable

  void updateDialogueTreeIfNeeded();
  void beginDialogue();
  void beginTrade();

  void onDialogueBegin();
  void onDialogueEnd();


  void act(float delta);
  void findNewLockedOnTargetFromParty(Character* killedTarget);
  void moveToTarget(float delta, Character* target, float followDistance);
  void moveRandomly(float delta,
                    int minMoveDuration, int maxMoveDuration,
                    int minWaitDuration, int maxWaitDuration);
  void jumpIfStucked(float delta, float checkInterval);
  void reverseDirection();

  bool isInPlayerParty() const;
  bool isWaitingForPlayer() const;

  
  Npc::Profile& getNpcProfile();
  DialogueTree& getDialogueTree();
  Npc::Disposition getDisposition() const;
  bool isSandboxing() const;

  void setDisposition(Npc::Disposition disposition);
  void setSandboxing(bool sandboxing);

  static void setNpcsAllowedToAct(bool npcsAllowedToAct);

  static bool isNpcAllowedToSpawn(const std::string& jsonFileName);
  static void setNpcAllowedToSpawn(const std::string& jsonFileName, bool canSpawn);


 private:
  virtual void defineBody(b2BodyType bodyType,
                          float x,
                          float y,
                          short bodyCategoryBits=0,
                          short bodyMaskBits=0,
                          short feetMaskBits=0,
                          short weaponMaskBits=0) override;  // Character

  virtual void createHintBubbleFx() override;  // Interactable
  virtual void removeHintBubbleFx() override;  // Interactable


  // See `map/GameMap.cc` for its usage.
  static std::atomic<bool> _areNpcsAllowedToAct;

  // Once those spawn-once NPCs are killed, their jsonFileName
  // will be inserted into this unordered_set.
  static std::unordered_set<std::string> _npcSpawningBlacklist;

  Npc::Profile _npcProfile;
  DialogueTree _dialogueTree;
  Npc::Disposition _disposition;
  bool _isSandboxing;

  cocos2d::Sprite* _hintBubbleFxSprite;

  // The following variables are used in Npc::moveRandomly()
  bool _isMovingRight;
  float _moveDuration;
  float _moveTimer;
  float _waitDuration;
  float _waitTimer;

  // The following variables are used in Npc::jumpIfStucked()
  float _calculateDistanceTimer;
  b2Vec2 _lastStoppedPosition;
};

}  // namespace vigilante

#endif  // VIGILANTE_NPC_H_
