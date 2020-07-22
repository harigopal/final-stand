open Belt;

let scale = 3;
let numberOfTilesInARow = 5;
let tileHeight = 20 * scale;
let tileWidth = 20 * scale;

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
    htmlElem;
  };
};
let px = i => i->string_of_int ++ "px";

let root = Document.getElementById("root", document) |> Option.getUnsafe;
let gameWorldDom =
  Document.getElementById("game-world", document) |> Option.getUnsafe;
let gameControlsDom =
  Document.getElementById("game-controls", document) |> Option.getUnsafe;
let logsDom = Document.getElementById("logs", document) |> Option.getUnsafe;

let render = (world: World.t) => {
  Element.setInnerText(
    logsDom,
    Js.Json.stringifyAny(world)
    ->Option.getWithDefault("Could not stringify"),
  );

  WorldDomRenderer.render(gameWorldDom, world);
};

let incrButton = Document.createElement("button", document);
incrButton->Element.setInnerText("Next Turn");
incrButton->Element.appendChild(gameControlsDom);

incrButton->Element.setOnClick(_evt => {
  world := World.endPlayerTurn(world^);
  render(world^);
});

render(world^);
