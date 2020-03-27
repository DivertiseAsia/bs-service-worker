type t = Js.t({.
  scriptURL: string,
  state: string,
});

module Container {
  type t = Js.t({.
    controller: option(t)
  });
}

module Registration {
  type t = Js.t({
    .
    scope: string,
    updateViaCache: string,
    [@bs.meth] unregister: unit => Js.Promise.t(bool),
  });
}

module Window {
  type t;
  [@bs.scope "window"][@bs.val] external addEventListener : (string, unit => 'a) => unit = "addEventListener";
};

[@bs.scope "navigator"] [@bs.val] external maybeServiceWorker: option(Container.t) = "serviceWorker";
[@bs.send] external register: (Container.t, string) => Js.Promise.t(Registration.t) = "register";

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