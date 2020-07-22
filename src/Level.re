type t = {enemies: array(Enemy.t)};

/* So BuckleScript is no longer a "functional" language, since the
   primary collection is a mutable, reference-based structure. */
let getEnemies = t => Belt.Array.concat([||], t.enemies);

let make = (~enemies) => {
  {enemies: enemies};
};
