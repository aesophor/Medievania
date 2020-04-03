// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "GameMapManager.h"

#include <Box2D/Box2D.h>
#include "AssetManager.h"
#include "Constants.h"
#include "character/Player.h"
#include "character/Enemy.h"
#include "item/Equipment.h"
#include "skill/MagicalMissile.h"
#include "util/box2d/b2BodyBuilder.h"

using std::set;
using std::string;
using std::unique_ptr;
using cocos2d::Director;
using cocos2d::Layer;
using cocos2d::TMXTiledMap;
using cocos2d::TMXObjectGroup;

namespace vigilante {

GameMapManager* GameMapManager::getInstance() {
  static GameMapManager instance({0, kGravity});
  return &instance;
}

GameMapManager::GameMapManager(const b2Vec2& gravity)
    : _layer(Layer::create()),
      _worldContactListener(new WorldContactListener()),
      _world(new b2World(gravity)),
      _fxMgr(new FxManager(_layer)),
      _gameMap(),
      _player() {
  _world->SetAllowSleeping(true);
  _world->SetContinuousPhysics(true);
  _world->SetContactListener(_worldContactListener.get());
}

void GameMapManager::update(float delta) {
  if (_player) {
    _player->update(delta);
  }
  for (auto actor : _gameMap->getDynamicActors()) {
    actor->update(delta);
  }
}


GameMap* GameMapManager::getGameMap() const {
  return _gameMap.get();
}

void GameMapManager::loadGameMap(const string& tmxMapFileName) {
  // Clean up previous GameMap.
  if (_gameMap) {
    _layer->removeChild(_gameMap->getTmxTiledMap());
    _gameMap->deleteObjects();
    _gameMap.reset(); // deletes the underlying GameMap object
  }

  _gameMap = unique_ptr<GameMap>(new GameMap(_world.get(), tmxMapFileName));
  _gameMap->createObjects();
  _layer->addChild(_gameMap->getTmxTiledMap(), graphical_layers::kTmxTiledMap);

  // If the player object hasn't been created, spawn it.
  if (!_player) {
    _player = unique_ptr<Player>(_gameMap->createPlayer());
  }
}


Player* GameMapManager::getPlayer() const {
  return _player.get();
}

b2World* GameMapManager::getWorld() const {
  return _world.get();
}

Layer* GameMapManager::getLayer() const {
  return _layer;
}


void GameMapManager::createDustFx(Character* character) {
  auto feetPos = character->getBody()->GetPosition();
  float x = feetPos.x * kPpm;// - 32.f / kPpm / 2;
  float y = (feetPos.y - .1f) * kPpm;// - 32.f / kPpm / .065f;
  _fxMgr->createFx("Texture/fx/dust", "white", x, y);
}

} // namespace vigilante
