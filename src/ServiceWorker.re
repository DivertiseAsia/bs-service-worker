type serviceWorker = {
  scriptURL: string,
  state: string,
};

type serviceWorkerContainer = {
  controller: option(serviceWorker)
};

module Registration {
  type t = Js.t({
    .
    scope: string,
    updateViaCache: string,
    worker: option(serviceWorker),
    unregister: unit => Js.Promise.t(bool),
  });
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

[@bs.val] external registerJs: (string) => Js.Promise.t(Registration.t) = "navigator.serviceWorker.register";
[@bs.val] external _controller: Js.Nullable.t(serviceWorker) = "navigator.serviceWorker.controller"

exception RegistrationException(Js.Promise.error);
let register = (filename:string):Js.Promise.t(Registration.t) => {
  Js.Promise.(
    registerJs(filename)
    |> then_((b:Registration.t) => {
      resolve(b);
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
