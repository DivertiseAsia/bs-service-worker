type serviceWorker;
type navigator = {
  serviceWorker:option(serviceWorker)
};

[@bs.val] external navigator: navigator = "navigator";

let supportsServiceWorker = ():bool => {
  switch(navigator.serviceWorker) {
    | Some(x) => true;
    | None => false
  }
}

module Window {
  type window;
  [@bs.send]
  external addEventListener : (window, string, unit => unit) => unit =
    "addEventListener";
  [@bs.val] external window: window = "window";
};

let windowAddEventListener = (eventName:string, func:unit => unit):unit => {
  Window.addEventListener(Window.window, eventName, func);
};

type serviceWorkerRegistration = {
  scope:string,
  updateViaCache: string,
};

[@bs.val] external register: (string) => Js.Promise.t(serviceWorkerRegistration) = "navigator.serviceWorker.register";

