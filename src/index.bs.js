'use strict';


var WorldTime = {};

var Position = {};

var Tile = {};

var Enemy = {};

function getEnemies(t) {
  return [];
}

var Level = {
  getEnemies: getEnemies
};

var Tower = {};

function make(health, wealth, level) {
  return {
          startTime: new Date(),
          health: health,
          wealth: wealth,
          enemies: [],
          towers: []
        };
}

var World = {
  make: make
};

exports.WorldTime = WorldTime;
exports.Position = Position;
exports.Tile = Tile;
exports.Enemy = Enemy;
exports.Level = Level;
exports.Tower = Tower;
exports.World = World;
/* No side effect */
