/* 
This is a main service worker type. The javascript type is `ServiceWorker`.
But for the naming consistency, we will name it `controller`.
As the object is actually obtained from `serviceWorker.controller`
https://developer.mozilla.org/en-US/docs/Web/API/ServiceWorkerContainer/controller
*/
type messagePort = Js.t({.
  [@bs.meth] postMessage: (string) => unit
});

type messageEvent = Js.t({.
  data: string,
  origin: string,
  lastEventId: string,
  source: string,
  ports: Js.Array.t(messagePort)
});

module Controller {
  module State {
    type t;
    let installing:t;
    let installed:t;
    let activating:t;
    let activated:t;
    let redundant:t;
  }
  type t = Js.t({.
    scriptURL: string,
    state: State.t,
    [@bs.set] onerror : ((string) => unit) => unit,
    [@bs.set] onmessage : ((messageEvent) => unit) => unit,
    [@bs.set] onmessageerror : ((string) => unit) => unit
  });
}

module Registration {
  type t = Js.t({
    .
    scope: string,
    updateViaCache: string,
    active: Js.Nullable.t(Controller.t),
    [@bs.meth] unregister: unit => Js.Promise.t(bool),
  });
}

/* This Service worker Container */
module Container {
    type t = Js.t({.
      controller: Js.Nullable.t(Controller.t)
    });
  
    let register: (t, string) => Js.Promise.t(Registration.t);
    let getRegistration: (t) => Js.Promise.t(Registration.t);
};

module Window {
  type t;
  let addEventListener : (string, unit => 'a) => unit;
};

let maybeServiceWorker: option(Container.t);
let registerOnLoad : (Container.t, string) => Js.Promise.t(Registration.t);

exception RegistrationException(Js.Promise.error);


/*
// To check
switch(ServiceWorker.maybeServiceWorker){
| None => Js.log("[App] No ServiceWorker");
| Some(workerContainer) => Js.log2("Yes, there is a", worker##controller);
}

// To register
workerContainer->register("filename")
|> ... Handle promise ...

// To work with Registration.t
let handlerRegistartion = register => {
  Js.log2("[App] ServiceWorker registration success: ", register##scope);
}
*/