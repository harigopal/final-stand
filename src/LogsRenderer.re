open Belt;
open Webapi.Dom;

let logsDom = Document.getElementById("logs", document) |> Option.getUnsafe;

let render = world =>
  Element.setInnerText(
    logsDom,
    Js.Json.stringifyAny(world)
    ->Option.getWithDefault("Could not stringify"),
  );
