interface World {
  a: string;
  b: number;
}

enum Tile {
  Path,
  Buildable,
}

interface Map {
  [position: number]: Tile;
}
