open ReasonReact;

type state = {
  supported: bool,
};

type action =
  | Supported(bool);

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(
    (state, action) =>
      switch (action) {
        | Supported(supported) => {...state, supported}
      },
      {
        supported: false
      }
  );
  React.useEffect0(() => {
    if (ServiceWorker.supportsServiceWorker()) {
      dispatch(Supported(true));
      Js.log("[App] Browser supports service workers");
    } else {
      dispatch(Supported(false));
      Js.log("[App] Browser does *not* support service workers");
    }
    None;
  });
  <div>
    <h1>{string("hello service worker!")}</h1>
    <a href="features.html">{string("See Features")}</a>
    <h2>{string("Your Browser:")}</h2>
    <div>{string("Supports ServiceWorker?")} <span id="supported">{string(state.supported ? {j|✔|j} : {j|❌|j})}</span></div>
  </div>
}