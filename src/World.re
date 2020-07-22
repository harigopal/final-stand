open Belt;

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

let towers = t => t.towers;

let incrementTurn = t => {...t, turn: Turn.next(t.turn)};

let moveAliveEnemies = t => {
  {...t, enemies: t.enemies |> Js.Array.map(Enemy.move)};
};
let introduceQueuedEnemies = t => {
  {...t, enemies: t.enemies |> Js.Array.map(Enemy.tryToIntroduce(t.turn))};
};

let receiveDamage = t => {
  let totalDamage =
    Array.reduce(t.enemies, 0, (acc, enemy) =>
      Enemy.willDealDamage(enemy) ? acc + Enemy.damage(enemy) : acc
    );

  let enemies = t.enemies->Array.keep(enemy => !Enemy.willDealDamage(enemy));

  {...t, health: t.health - totalDamage, enemies};
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
    /* Order of conditions is significant */
    if (t.health <= 0) {
      GameState.lost;
    } else if (Belt.Array.every(t.enemies, Enemy.isDead)) {
      GameState.won;
    } else {
      GameState.inProgress;
    };

  {...t, gameState};
};

let endPlayerTurn = t => {
  t->runEnemyTurn->refreshGameState->incrementTurn;
};

let placeTower = (t, i) => {
  let towers =
    Array.concat(
      t.towers,
      [|Tower.make(~damage=50, ~position=Position.make(i))|],
    );
  {...t, towers};
};
