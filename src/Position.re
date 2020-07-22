type t = int;
let next = t => t + 1;
let initial: t = 1;

let isOutOfBounds = t => t > 10;

let numberOfTilesInARow = 5;

let coords = t => {
  let col = t mod numberOfTilesInARow + 1;
  let row = t / numberOfTilesInARow + 1;
  (row, col);
};

let make = (i: int): t => i;
