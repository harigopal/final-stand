open Belt;
open Webapi.Dom;

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
