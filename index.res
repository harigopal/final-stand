module WorldTime {
  type t;
}

module Position = {
  type t = int
}

module Tile = {
  type t = Path | Buildable
}

module Enemy {
  type vitalStats = {
    health: int,
    position: Position.t
  };

  type status = Queued | Live(vitalStats) | Dead(Position.t)

  type t = {
    entryTime: WorldTime.t,
    wealth: int,
    status
  }
}

module Level = {
   type t = {
     map: array(tile),
     enemies: array(Enemy.t)
   }
}

module Tower {
  type t = Above(int) | Below(int)
}

module World {
  type t =  {
    startTime: Js.Date.t,
    health: int,
    wealth: int,
    enemies: array(Enemy.t),
    towers: array(Tower.t),
  }

  let make = (~health, ~wealth, ~level) => {
    {
      health,
      wealth,
      enemies: Level.getEnemies(level),
      towers: []
    }
  }
}
