let render = (mapDom, world) => {
  /*
   1. Iterate over each tile in the map, and draw accordingly - Buildable/Path.
   2. Iterate over active Enemies and update their tile
    */

  Array.forEachWithIndex(
    map,
    (i, tile) => {
      let col = i mod numberOfTilesInARow + 1;
      let row = i / numberOfTilesInARow + 1;

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
          ~parent=mapDom,
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
        tileDom->HtmlElement.setOnClick(_event => {
          world := World.placeTower(world, i)
        })
      };
    },
  );
  ();
};
