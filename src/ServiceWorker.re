type serviceWorkerContainer;

module Navigator {
  type t = {
    serviceWorker:option(serviceWorkerContainer)
  };
  [@bs.val] external navigator: t = "navigator";
  let _supportsServiceWorker = ():bool => {
    switch(navigator.serviceWorker) {
      | Some(_) => true;
      | None => false
    }
  }
}

let isSupported = Navigator._supportsServiceWorker;

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

type serviceWorkerRegistration = {
  scope:string,
  updateViaCache: string,
};

[@bs.send] external unregister: (serviceWorkerRegistration) => Js.Promise.t(bool) = "unregister";

[@bs.val] external register: (string) => Js.Promise.t(serviceWorkerRegistration) = "navigator.serviceWorker.register";

