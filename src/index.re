open Belt;

module WorldTime = {
  type t = Js.Date.t;

  let forTick = (i: int) => {
    let t = Js.Date.make();
    Js.Date.setSeconds(t, i->float_of_int) |> ignore;
    t;
  };
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
    | Queued(WorldTime.t)
    | Live(Position.t)
    | Dead;

  let make = (~entryTime, ~health, ~wealth) => {
    {health, wealth, status: Queued(WorldTime.forTick(entryTime))};
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
    startTime: Js.Date.t,
    health: int,
    wealth: int,
    enemies: array(Enemy.t),
    towers: array(Tower.t),
  };

  let make = (~health, ~wealth, ~level) => {
    startTime: Js.Date.make(),
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
    Enemy.make(~entryTime=1, ~wealth=1, ~health=10),
    Enemy.make(~entryTime=5, ~wealth=1, ~health=10),
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
