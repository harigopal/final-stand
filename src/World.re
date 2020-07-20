type t = {
  gameState: GameState.t,
  turn: Turn.t,
  health: int,
  wealth: int,
  enemies: array(Enemy.t),
  towers: array(Tower.t),
};

let make = (~health, ~wealth, ~level) => {
  gameState: GameState.inProgress,
  turn: Turn.make(1),
  health,
  wealth,
  enemies: Level.getEnemies(level),
  towers: [||],
};

let incrementTurn = t => {...t, turn: Turn.next(t.turn)};

let moveAliveEnemies = t => {
  {...t, enemies: t.enemies |> Js.Array.map(Enemy.move)};
};
let introduceQueuedEnemies = t => {
  {...t, enemies: t.enemies |> Js.Array.map(Enemy.tryToIntroduce(t.turn))};
};

let receiveDamage = t => {
  let damaged = ref(false);

  let enemies =
    t.enemies
    |> Js.Array.filter(enemy =>
         if (Enemy.willDealDamage(enemy)) {
           damaged := true;
           false;
         } else {
           true;
         }
       );

  {...t, health: damaged^ ? t.health - 20 : t.health, enemies};
};

let runEnemyTurn = t => {
  /*
   * Compute damage to enemies from towers
   * Kill enemies whose health = 0
   * Move alive enemies
   * Appear enemies who're queued for this turn
   */
  t
  ->moveAliveEnemies
  ->receiveDamage
  ->introduceQueuedEnemies;
};

let refreshGameState = t => {
  let gameState =
    if (Belt.Array.every(t.enemies, Enemy.isDead)) {
      GameState.won;
    } else if (t.health > 0) {
      GameState.inProgress;
    } else {
      GameState.lost;
    };

  {...t, gameState};
};

let endPlayerTurn = t => {
  t->runEnemyTurn->refreshGameState->incrementTurn;
};
