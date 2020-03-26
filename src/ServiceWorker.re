type serviceWorker = {
  scriptURL: string,
  state: string,
};

type serviceWorkerContainer = {
  controller: option(serviceWorker)
};

module ServiceWorkerRegistration {
  type js = {
    scope:string,
    updateViaCache: string,
    active: Js.Nullable.t(serviceWorker),
    installing: Js.Nullable.t(serviceWorker),
    waiting: Js.Nullable.t(serviceWorker),
  };
  type t = {
    .
    scope: string,
    updateViaCache: string,
    worker: option(serviceWorker),
    unregister: unit => Js.Promise.t(bool),
  };
  [@bs.send] external _unregister: (js) => Js.Promise.t(bool) = "unregister";
  let _getRawWorker(registration:js) = {
    switch (Js.Nullable.toOption(registration.active), 
      Js.Nullable.toOption(registration.installing), 
      Js.Nullable.toOption(registration.waiting)) {
      | (Some(x), _, _) => ("active", Some(x))
      | (None, Some(x), _) => ("installing", Some(x))
      | (None, None, Some(x)) => ("waiting", Some(x))
      | _ => ("", None)
    };
  };
  let jsToLib = (jsRecord:js):t => {
    let (initialState, worker) = _getRawWorker(jsRecord);
    let obj:t = {
      pub scope = jsRecord.scope;
      pub updateViaCache = jsRecord.updateViaCache;
      pub worker = worker;
      val raw = jsRecord;
      pub unregister = () => {
        _unregister(raw)
      }
    };
    obj;
  };
}

module Navigator {
  type t = {
    serviceWorker:option(serviceWorkerContainer),
  };
  [@bs.val] external navigator: t = "navigator";
  let _supportsServiceWorker = ():bool => {
    switch(navigator.serviceWorker) {
      | Some(_) => true;
      | None => false
    }
  }
}

module Window {
  type t;
  [@bs.send]
  external addEventListener : (t, string, unit => 'a) => unit =
    "addEventListener";
  [@bs.val] external window: t = "window";
};

[@bs.send] external unregisterJs: (ServiceWorkerRegistration.js) => Js.Promise.t(bool) = "unregister";
[@bs.val] external registerJs: (string) => Js.Promise.t(ServiceWorkerRegistration.js) = "navigator.serviceWorker.register";
[@bs.val] external _controller: Js.Nullable.t(serviceWorker) = "navigator.serviceWorker.controller"

exception RegistrationException(Js.Promise.error);
let register = (filename:string):Js.Promise.t(ServiceWorkerRegistration.t) => {
  Js.Promise.(
    registerJs(filename)
    |> then_((b:ServiceWorkerRegistration.js) => {
      resolve(ServiceWorkerRegistration.jsToLib(b));
    })
    |> catch(e => {
      reject(RegistrationException(e))
    })
  )
};
let isSupported = Navigator._supportsServiceWorker;
let getController = () => Js.Nullable.toOption(_controller);

let windowAddEventListener = (eventName:string, func):unit => {
  Window.addEventListener(Window.window, eventName, func);
};
