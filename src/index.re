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
  type vitalStats = {
    health: int,
    position: Position.t,
  };

  type status =
    | Queued
    | Live(vitalStats)
    | Dead(Position.t);

  type t = {
    entryTime: WorldTime.t,
    wealth: int,
    status,
  };

  let make = (~entryTime, ~wealth) => {
    {entryTime: WorldTime.forTick(entryTime), wealth, status: Queued};
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
  type t =
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
    Path,
    Path,
    Path,
    Buildable,
  |];
  let enemies = [|
    Enemy.make(~entryTime=1, ~wealth=1),
    Enemy.make(~entryTime=5, ~wealth=1),
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
