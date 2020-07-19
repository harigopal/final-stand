open Belt;

module Turn = {
  type t = int;
  let initial: t = 0;

  let make = (i: int): t => i;
};

module Position = {
  type t = int;
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

module World = {
  type t = {
    turn: Turn.t,
    health: int,
    wealth: int,
    enemies: array(Enemy.t),
    towers: array(Tower.t),
  };

  let make = (~health, ~wealth, ~level) => {
    turn: Turn.make(1),
    health,
    wealth,
    enemies: Level.getEnemies(level),
    towers: [||],
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
  let world = World.make(~health=100, ~wealth=8, ~level);

  open Webapi.Dom;
  let root = Document.getElementById("root", document) |> Option.getUnsafe;

  Element.setInnerText(
    root,
    Js.Json.stringifyAny(world)
    ->Option.getWithDefault("Could not stringify"),
  );
};
