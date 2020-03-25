open ReasonReact;

type state = {
  supported: bool,
  registerSuccess: bool,
  registration: option(ServiceWorker.serviceWorkerRegistration),
  failureSuccess: bool,
  unregisterSuccess: bool,
};

type action =
  | Supported(bool)
  | Registered(ServiceWorker.serviceWorkerRegistration)
  | UnregisterSuccess
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
        | Registered(registration) => {...state, registerSuccess: true, registration: Some(registration)}
        | UnregisterSuccess => {...state, unregisterSuccess: true, registration: None}
        | SuccessfulFailure => {...state, failureSuccess:true}
      },
      {
        supported: false, 
        registerSuccess: false,
        registration: None,
        unregisterSuccess: false,
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
        ) |> ignore;
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
  let unregisterServiceWorker = (_) => {
    switch (state.registration) {
      | Some(registration) => {
        Js.Promise.(ServiceWorker.unregister(registration)
          |> then_((success:bool) => {
            if (success == true) {
              Js.log("[App] ServiceWorker unregister success");
              dispatch(UnregisterSuccess);
            } else {
              Js.log("[App] ServiceWorker unregister failed");
            }
            resolve(success);
          })
        ) |> ignore;
      }
      | None => {
        Js.log("[App] No ServiceWorker to unregister")
      }
    }
  };
  <div>
    <h1>{string("hello service worker! (reason)")}</h1>
    <a href="features.html">{string("See Features")}</a>
    <h2>{string("Your Browser:")}</h2>
    <table>
      <tbody>
        <tr key="supported"><td>{string("Supports ServiceWorker?")}</td><td>{successIndicator(state.supported, "supported")}</td></tr>
        <tr key="registration"><td>{string("Registered ServiceWorker now?")}</td><td>{successIndicator(state.registerSuccess, "registerSuccess")}</td></tr>
        <tr key="failureSuccess"><td>{string("Registering Nonexistant ServiceWorker fails?")}</td><td>{successIndicator(state.failureSuccess, "failureSuccess")}</td></tr>
        <tr key="unregisterSuccess">
          <td>{string("Unregister works?")}</td>
          <td>{state.registration !== None ? <button id="unregister" onClick=unregisterServiceWorker>{string("Unregister")}</button> : successIndicator(state.unregisterSuccess, "unregisterSuccess")}</td>
        </tr>
      </tbody>
    </table>
  </div>
}