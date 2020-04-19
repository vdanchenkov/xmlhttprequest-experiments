open XmlHttpRequest;

// First Scenario, as described by ReasonML twitter:
// https://twitter.com/reasonml/status/1238026671581622272

// Very simple, we do one dog fetching request

[@bs.scope "JSON"] [@bs.val]
external parseResponse:
  Js.Nullable.t(string) => {. "message": array(string)} =
  "parse";

// ================ real parallel example to that linked file now

type state =
  | LoadingDogs
  | ErrorFetchingDogs
  | LoadedDogs(array(string));

[@react.component]
let make = () => {
  let (state, setState) = React.useState(() => LoadingDogs);

  // Notice that instead of `useEffect`, we have `useEffect0`. See
  // reasonml.github.io/reason-react/docs/en/components#hooks for more info
  React.useEffect0(() => {
    let xhr = make();
    xhr->onLoad(_ => {
      setState(_previousState =>
        LoadedDogs(xhr->responseText->parseResponse##message)
      )
    });
    xhr->onError(_ => {setState(_previousState => ErrorFetchingDogs)});
    xhr->open_(~method="GET", ~url="/api/dogs_success");

    xhr->setRequestHeader("Content-Type", "application/json");
    xhr->send;

    // the return value is called by React's useEffect when the component unmounts
    Some(() => {xhr->abort});
  });

  <div className="grid grid-cols-3 col-gap-4">
    {switch (state) {
     | ErrorFetchingDogs => React.string("An error occurred!")
     | LoadingDogs => React.string("Loading...")
     | LoadedDogs(dogs) =>
       dogs
       ->Belt.Array.mapWithIndex((i, dog) => {
           <img key=dog className="rounded-lg object-cover h-40" src=dog />;
         })
       ->React.array
     }}
  </div>;
};
