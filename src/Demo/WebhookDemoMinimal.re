open ReasonReact;

type state = {
  supported: bool,
  registration: option(ServiceWorker.ServiceWorkerRegistration.t),
  unregisterSuccess: bool,
};

type action =
  | Supported
  | Registered(ServiceWorker.ServiceWorkerRegistration.t)
  | UnregisterSuccess;

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
        | Supported => {...state, supported: true}
        | Registered(registration) => {...state, registration: Some(registration)}
        | UnregisterSuccess => {...state, unregisterSuccess: true}
      },
      {
        supported: false, 
        registration: None,
        unregisterSuccess: false,
      }
  );
  React.useEffect0(() => {
    if (ServiceWorker.isSupported()) {
      dispatch(Supported);
      ServiceWorker.windowAddEventListener("load", () => {
        Js.Promise.(ServiceWorker.register("demo-sw.js")
          |> then_((b:ServiceWorker.ServiceWorkerRegistration.t) => {
            dispatch(Registered(b))
            resolve(Some(b));
          })
          |> catch(e => {
            Js.log2("[App] ServiceWorker registration failed (expected): ", e);
            resolve(None)
          })
        ) |> ignore;
      })
    }
    None;
  });
  let unregisterServiceWorker = (_) => {
    switch (state.registration) {
      | Some(registration) => {
        Js.Promise.(registration#unregister()
          |> then_((success:bool) => {
            if (success == true) {
              Js.log("[App] ServiceWorker unregister success");
              dispatch(UnregisterSuccess);
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
    <h1>{string("Minimal Service Worker Demo")}</h1>
    <a href="features.html">{string("See Features")}</a>
    <h2>{string("Your Browser:")}</h2>
    <table>
      <tbody>
        <tr key="supported"><td>{string("Supports ServiceWorker?")}</td><td>{successIndicator(state.supported, "supported")}</td></tr>
        <tr key="registration"><td>{string("Registered ServiceWorker?")}</td><td>{successIndicator(state.registration !== None, "registerSuccess")}</td></tr>
        <tr key="unregisterSuccess">
          <td>{string("Unregister?")}</td>
          <td>{switch(state.registration, state.unregisterSuccess) {
            | (Some(_), false) => <button id="unregister" onClick=unregisterServiceWorker>{string("Unregister")}</button>
            | _ => successIndicator(state.unregisterSuccess, "unregisterSuccess")
          }}
          </td>
        </tr>
      </tbody>
    </table>
  </div>
}