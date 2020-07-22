open Belt;
open Webapi.Dom;

let scale = 3;
let towerHeight = 10 * scale;
let towerWidth = 10 * scale;
let tileHeight = 20 * scale;
let tileWidth = 20 * scale;

let gameWorldDom =
  Document.getElementById("game-world", document) |> Option.getUnsafe;

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

let px = i => i->string_of_int ++ "px";

let mapRender = handlePlaceTower => {
  Array.forEachWithIndex(
    map,
    (i, tile) => {
      let (row, col) = Position.coords(i);
      let className =
        "tile "
        ++ (
          switch (tile) {
          | Buildable => "tile-buildable"
          | Path => "tile-path"
          }
        );

      let tileDom =
        FSDom.make(
          ~elem="div",
          ~parent=gameWorldDom,
          ~className,
          ~css=[|
            ("width", px(tileWidth - 2)),
            ("height", px(tileHeight - 2)),
            ("top", px(row * tileHeight)),
            ("left", px(col * tileWidth)),
          |],
        );

      switch (tile) {
      | Path => ()
      | Buildable =>
        tileDom->HtmlElement.setOnClick(_event => handlePlaceTower(i))
      };
    },
  );
};

let towerRender = towers => {
  let aux = tower => {
    let (row, col) = Position.coords(Tower.position(tower));

    FSDom.make(
      ~elem="div",
      ~parent=gameWorldDom,
      ~className="tower",
      ~css=[|
        ("width", px(towerWidth - 2)),
        ("height", px(towerHeight - 2)),
        ("top", px(row * tileHeight)),
        ("left", px(col * tileWidth)),
      |],
    )
    |> ignore;
  };
  Array.forEach(towers, aux);
};

/*
 1. Iterate over each tile in the map, and draw accordingly - Buildable/Path.
 3. Iterate over active Towers and render their layer tile
 2. Iterate over active Enemies
  */
let render = (~world, ~handlePlaceTower) => {
  mapRender(handlePlaceTower);
  towerRender(World.towers(world));
};
