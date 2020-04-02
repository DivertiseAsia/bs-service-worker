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
  module State : {
    type t;
    let installing:t;
    let installed:t;
    let activating:t;
    let activated:t;
    let redundant:t;
  } = {
      type t = string;
      let installing:t = "installing";
      let installed:t = "installed";
      let activating:t = "activating";
      let activated:t = "activated";
      let redundant:t = "redundant";
  };
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

module Container {
  type t = Js.t({.
    controller: Js.Nullable.t(Controller.t)
  });

  [@bs.send] external register: (t, string) => Js.Promise.t(Registration.t) = "register";
  [@bs.send] external getRegistration: (t) => Js.Promise.t(Registration.t) = "getRegistration";
}

module Window {
  type t;
  [@bs.scope "window"][@bs.val] external addEventListener : (string, unit => 'a) => unit = "addEventListener";
};

[@bs.scope "navigator"] [@bs.val] external maybeServiceWorker: option(Container.t) = "serviceWorker";

exception RegistrationException(Js.Promise.error);
let registerOnLoad = (container:Container.t, serviceWorkerFile:string):Js.Promise.t(Registration.t) => {
  Js.Promise.make(
     (~resolve as upper, ~reject as upperReject) => {
      Window.addEventListener("load", () => {
        (Js.Promise.(
          Container.register(container, serviceWorkerFile)
          |> then_((b:Registration.t) => {
            [@bs] upper(b);
            resolve()
          })
          |> catch((e) => {
            [@bs] upperReject(RegistrationException(e))
            resolve()
          })
        ))
      });
    }
  )
};

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