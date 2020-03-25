type serviceWorker = {
  scriptURL: string,
  state: string,
};

type serviceWorkerContainer = {
  controller: option(serviceWorker)
};

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
[@bs.val] external _controller: Js.Nullable.t(serviceWorker) = "navigator.serviceWorker.controller"

let isSupported = Navigator._supportsServiceWorker;
let getController = () => Js.Nullable.toOption(_controller);

module Window {
  type t;
  [@bs.send]
  external addEventListener : (t, string, unit => 'a) => unit =
    "addEventListener";
  [@bs.val] external window: t = "window";
};

let windowAddEventListener = (eventName:string, func):unit => {
  Window.addEventListener(Window.window, eventName, func);
};

module ServiceWorkerRegistration {
  type raw = {
    scope:string,
    updateViaCache: string,
    active: Js.Nullable.t(serviceWorker),
    installing: Js.Nullable.t(serviceWorker),
    waiting: Js.Nullable.t(serviceWorker),
  };
  type t = {
    scope:string,
    updateViaCache: string,
    worker: option(serviceWorker),
    initialState: string,
    raw: raw,
  };
  let _getRawWorker(registration:raw) = {
    switch (Js.Nullable.toOption(registration.active), 
      Js.Nullable.toOption(registration.installing), 
      Js.Nullable.toOption(registration.waiting)) {
      | (Some(x), _, _) => ("active", Some(x))
      | (None, Some(x), _) => ("installing", Some(x))
      | (None, None, Some(x)) => ("waiting", Some(x))
      | _ => ("", None)
    };
  };
  let jsToTyped = (raw:raw):t => {
    let (initialState, worker) = _getRawWorker(raw);
    {
      scope: raw.scope,
      updateViaCache: raw.updateViaCache,
      worker,
      initialState,
      raw
    }
  };
}


[@bs.send] external unregister: (ServiceWorkerRegistration.raw) => Js.Promise.t(bool) = "unregister";

[@bs.val] external register: (string) => Js.Promise.t(ServiceWorkerRegistration.raw) = "navigator.serviceWorker.register";

