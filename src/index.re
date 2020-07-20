open Belt;

module Turn = {
  type t = int;
  let initial: t = 1;

  let make = (i: int): t => i;

  let next = t => t + 1;

  let isSame = (x, y) => x == y;
};

module Position = {
  type t = int;
  let next = t => t + 1;
  let initial: t = 1;
};

module Tile = {
  type t =
    | Path
    | Buildable;
};

module Enemy = {
  type t = {
    health: int,
    wealth: int,
    status,
  }
  and status =
    | Queued(Turn.t)
    | Live(Position.t)
    | Dead;

  let make = (~turn, ~health, ~wealth) => {
    {health, wealth, status: Queued(turn)};
  };

  let tryToIntroduce = (turn: Turn.t, t) => {
    let status =
      switch (t.status) {
      | Live(_)
      | Dead => t.status
      | Queued(enemyTurn) =>
        Turn.isSame(turn, enemyTurn) ? Live(Position.initial) : t.status
      };
    {...t, status};
  };

  let move = t => {
    {
      ...t,
      status:
        switch (t.status) {
        | Queued(_)
        | Dead => t.status
        | Live(position) => Live(Position.next(position))
        },
    };
  };

  let isDead = t =>
    switch (t.status) {
    | Queued(_)
    | Live(_) => false
    | Dead => true
    };
};

module Level = {
  type t = {
    map: array(Tile.t),
    enemies: array(Enemy.t),
  };

  /* So BuckleScript is no longer a "functional" language, since the
     primary collection is a mutable, reference-based structure. */
  let getEnemies = t => Array.concat([||], t.enemies);

  let make = (~map, ~enemies) => {map, enemies};
};

module Tower = {
  type t = {
    damage: int,
    position,
  }
  and position =
    | Above(int)
    | Below(int);
};

module GameState = {
  type t =
    | InProgress
    | Won
    | Lost;

  let inProgress = InProgress;
  let won = Won;
  let lost = Lost;
};

module World = {
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
    {...t, enemies: t.enemies->Array.map(Enemy.move)};
  };
  let introduceQueuedEnemies = t => {
    {...t, enemies: t.enemies->Array.map(Enemy.tryToIntroduce(t.turn))};
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
    ->introduceQueuedEnemies;
  };

  let refreshGameState = t => {
    let gameState =
      if (Belt.Array.every(t.enemies, Enemy.isDead)) {
        GameState.won;
      } else if (t.health == 0) {
        GameState.lost;
      } else {
        GameState.inProgress;
      };

    {...t, gameState};
  };

  let endPlayerTurn = t => {
    t->runEnemyTurn->refreshGameState->incrementTurn;
  };
};

module Main = {
  let map: array(Tile.t) = [|
    Buildable,
    Buildable,
    Buildable,
    Buildable,
    Buildable,
    Path,
    Path,
    Path,
    Path,
    Path,
    Buildable,
    Buildable,
    Buildable,
    Buildable,
    Buildable,
  |];

  let enemies = [|
    Enemy.make(~turn=1, ~wealth=1, ~health=10),
    Enemy.make(~turn=5, ~wealth=1, ~health=10),
    Enemy.make(~turn=8, ~wealth=1, ~health=10),
    Enemy.make(~turn=12, ~wealth=1, ~health=10),
    Enemy.make(~turn=13, ~wealth=1, ~health=10),
    Enemy.make(~turn=14, ~wealth=1, ~health=10),
    Enemy.make(~turn=18, ~wealth=1, ~health=10),
    Enemy.make(~turn=19, ~wealth=1, ~health=10),
    Enemy.make(~turn=20, ~wealth=1, ~health=10),
  |];

  let level = Level.make(~map, ~enemies);
  let world = ref(World.make(~health=100, ~wealth=8, ~level));

  open Webapi.Dom;
  let root = Document.getElementById("root", document) |> Option.getUnsafe;

  let domLogElem = Document.createElement("p", document);
  Element.setId(domLogElem, "logConsole");
  Element.appendChild(domLogElem, root);

  let render = (world: World.t) => {
    Element.setInnerText(
      domLogElem,
      Js.Json.stringifyAny(world)
      ->Option.getWithDefault("Could not stringify"),
    );
  };

  let incrButton = Document.createElement("button", document);
  incrButton->Element.setInnerText("Next Turn");
  incrButton->Element.appendChild(root);
  incrButton->Element.setOnClick(_evt => {
    world := World.endPlayerTurn(world^);
    render(world^);
  });

  render(world^);
};
