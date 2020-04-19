open Util.ReactStuff;

[@bs.scope "JSON"] [@bs.val]
external parseResponse:
  Js.Nullable.t(string) => {. "message": array(string)} =
  "parse";

type state =
  | NotAsked
  | Ready({start: unit => unit})
  | Pending({cancel: unit => unit})
  | Success({
      restart: unit => unit,
      data: array(string),
    })
  | Failed(string);

[@react.component]
let make = () => {
  let (state, setState) = React.useState(() => NotAsked);

  React.useEffect0(() => {
    open XmlHttpRequest;
    let xhr = make();

    let start = () => {
      xhr->open_(~method="POST", ~url="/api/dogs_success");
      xhr->setRequestHeader("Content-Type", "application/json");
      xhr->sendString(
        {"timeout": 5000}
        ->Js.Json.stringifyAny
        ->Belt.Option.getWithDefault(""),
      );
    };

    xhr->onLoad(_ => {
      setState(_previousState =>
        Success({
          restart: start,
          data: xhr->responseText->parseResponse##message,
        })
      )
    });

    xhr->onLoadStart(_ =>
      setState(_previousState => Pending({cancel: () => xhr->abort}))
    );

    xhr->onAbort(_ => setState(_previousState => Ready({start: start})));
    xhr->onError(_ =>
      setState(_previousState => Failed("Connection fail (or something)"))
    );

    setState(_ => Ready({start: start}));

    // the return value is called by React's useEffect when the component unmounts
    Some(() => {xhr->abort});
  });

  <div>
    {switch (state) {
     | NotAsked => <div> "Getting ready..."->s </div>
     | Ready({start}) =>
       <button onClick={_ => start()}>
         "Start request (takes 5 sec)"->s
       </button>
     | Pending({cancel}) =>
       <div>
         "Waiting for data"->s
         <button onClick={_ => cancel()}> "Cancel"->s </button>
       </div>
     | Success({restart}) =>
       <div>
         "Found some doggos"->s
         <button onClick={_ => restart()}> "Restart"->s </button>
       </div>
     | Failed(err) =>
       <div>
         {{
            "Error: " ++ err;
          }
          ->s}
       </div>
     }}
  </div>;
};
