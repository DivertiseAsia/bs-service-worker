open ReasonReact;

type state = {
  supported: bool,
  registration: option(ServiceWorker.serviceWorkerRegistration),
  failureSuccess: bool,
};

type action =
  | Supported(bool)
  | Registered(ServiceWorker.serviceWorkerRegistration)
  | SuccessfulFailure;

let successIndicator = (condition:bool, id:string) => {
  <span id=id key=id>
    {string(condition ? {j|✔|j} : {j|❌|j})}
  </span>
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(
    (state, action) =>
      switch (action) {
        | Supported(supported) => {...state, supported}
        | Registered(registration) => {...state, registration: Some(registration)}
        | SuccessfulFailure => {...state, failureSuccess:true}
      },
      {
        supported: false, 
        registration: None,
        failureSuccess: false,
      }
  );
  React.useEffect0(() => {
    if (ServiceWorker.isSupported()) {
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
        Js.Promise.(ServiceWorker.register("nonexistant-sw.js")
          |> then_((b:ServiceWorker.serviceWorkerRegistration) => {
            resolve(Some(b));
          })
          |> catch(e => {
            Js.log2("[App] ServiceWorker registration failed (expected): ", e);
            dispatch(SuccessfulFailure);
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
    <table>
      <tbody>
        <tr key="supported"><td>{string("Supports ServiceWorker?")}</td><td>{successIndicator(state.supported, "supported")}</td></tr>
        <tr key="registration"><td>{string("Registered ServiceWorker now?")}</td><td>{successIndicator(state.registration !== None, "registration")}</td></tr>
        <tr key="failureSuccess"><td>{string("Registering Nonexistant ServiceWorker fails?")}</td><td>{successIndicator(state.failureSuccess, "failureSuccess")}</td></tr>
      </tbody>
    </table>
  </div>
}