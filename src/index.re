open Belt;

let numberOfTilesInARow = 5;
let tileHeight = 20;
let tileWidth = 20;

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
  Enemy.make(~turn=1, ~wealth=1, ~health=10, ~damage=20),
  Enemy.make(~turn=5, ~wealth=1, ~health=10, ~damage=10),
  Enemy.make(~turn=8, ~wealth=1, ~health=10, ~damage=20),
  Enemy.make(~turn=12, ~wealth=1, ~health=10, ~damage=10),
  Enemy.make(~turn=13, ~wealth=1, ~health=10, ~damage=20),
  Enemy.make(~turn=14, ~wealth=1, ~health=10, ~damage=10),
  Enemy.make(~turn=18, ~wealth=1, ~health=10, ~damage=20),
  Enemy.make(~turn=19, ~wealth=1, ~health=10, ~damage=20),
  Enemy.make(~turn=20, ~wealth=1, ~health=10, ~damage=20),
|];

let level = Level.make(~map, ~enemies);
let world = ref(World.make(~health=100, ~wealth=8, ~level));

open Webapi.Dom;
let root = Document.getElementById("root", document) |> Option.getUnsafe;

let gameWorldDom = Document.createElement("div", document);
Element.setId(gameWorldDom, "gameWorld");
Element.appendChild(gameWorldDom, root);

let domLogElem = Document.createElement("p", document);
Element.setId(domLogElem, "logConsole");
Element.appendChild(domLogElem, root);

module FSDom = {
  let make = (~elem, ~parent, ~className="", ~css=[||]) => {
    let d = Document.createElement(elem, document);
    d->Element.setClassName(className);
    let htmlElem = d->HtmlElement.ofElement->Option.getUnsafe;
    let style = htmlElem->HtmlElement.style;
    Array.forEach(css, ((cssProp, cssVal)) => {
      style |> CssStyleDeclaration.setProperty(cssProp, cssVal, "")
    });
    Element.appendChild(htmlElem, parent);
  };
};
let px = i => i->string_of_int ++ "px";

module WorldDomRenderer = {
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
          switch (tile) {
          | Buildable => "tile-buildable"
          | Path => "tile-path"
          };

        FSDom.make(
          ~elem="div",
          ~parent=mapDom,
          ~className,
          ~css=[|
            ("top", px(row * tileHeight + 100)),
            ("left", px(col * tileWidth + 100)),
          |],
        );
      },
    );
    ();
  };
};
let render = (world: World.t) => {
  Element.setInnerText(
    domLogElem,
    Js.Json.stringifyAny(world)
    ->Option.getWithDefault("Could not stringify"),
  );

  WorldDomRenderer.render(gameWorldDom, world);
};

let incrButton = Document.createElement("button", document);
incrButton->Element.setInnerText("Next Turn");
incrButton->Element.appendChild(root);
incrButton->Element.setOnClick(_evt => {
  world := World.endPlayerTurn(world^);
  render(world^);
});

render(world^);
