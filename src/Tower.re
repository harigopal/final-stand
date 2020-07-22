type t = {
  damage: int,
  position: Position.t,
};

let position = t => t.position;

let make = (~damage, ~position) => {damage, position};
