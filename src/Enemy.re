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

let willDealDamage = t =>
  switch (t.status) {
  | Queued(_)
  | Dead => false
  | Live(position) => Position.isOutOfBounds(position)
  };
