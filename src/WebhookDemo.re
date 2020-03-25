open ReasonReact;

type state = {
  supported: bool,
  registration: option(ServiceWorker.serviceWorkerRegistration),
};

type action =
  | Supported(bool)
  | Registered(ServiceWorker.serviceWorkerRegistration);

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(
    (state, action) =>
      switch (action) {
        | Supported(supported) => {...state, supported}
        | Registered(registration) => {...state, registration: Some(registration)}
      },
      {
        supported: false, 
        registration: None,
      }
  );
  React.useEffect0(() => {
    if (ServiceWorker.supportsServiceWorker()) {
      Js.log("[App] Browser supports service workers");
      dispatch(Supported(true));
      ServiceWorker.windowAddEventListener("load", () => {
        Js.Promise.(ServiceWorker.register("demo-sw.js")
          |> then_((b:ServiceWorker.serviceWorkerRegistration) => {
            Js.log("[App] ServiceWorker registration successful with scope: " ++ b.scope);
            dispatch(Registered(b))
            resolve(Some(b));
          })
          |> catch(e => {
            Js.log2("[App] ServiceWorker registration failed: ", e);
            resolve(None)
          })
        );
      })
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
    <div>{string("Registered ServiceWorker now?")} <span id="registered">{string(state.registration !== None ? {j|✔|j} : {j|❌|j})}</span></div>
  </div>
}