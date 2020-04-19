%raw
"require('../styles/main.css')";

open Util.ReactStuff;

open XmlHttpRequest;

[@bs.val] [@bs.scope "process"]
external isBrowser: option(bool) = "isBrowser";

[@react.component]
let default = () =>
  <div>
    <h1 className="text-3xl font-semibold">
      "What is this about?"->ReasonReact.string
    </h1>
    <div> <SimpleExample /> <HorizontalLine /> <CancelExample /> </div>
  </div>;
