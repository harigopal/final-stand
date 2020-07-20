open Belt;

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

let domLogElem = Document.createElement("p", document);
Element.setId(domLogElem, "logConsole");
Element.appendChild(domLogElem, root);

let render = (world: World.t) => {
  Element.setInnerText(
    domLogElem,
    Js.Json.stringifyAny(world)
    ->Option.getWithDefault("Could not stringify"),
  );
};

let incrButton = Document.createElement("button", document);
incrButton->Element.setInnerText("Next Turn");
incrButton->Element.appendChild(root);
incrButton->Element.setOnClick(_evt => {
  world := World.endPlayerTurn(world^);
  render(world^);
});

render(world^);
