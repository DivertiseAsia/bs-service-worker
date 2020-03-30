switch(ServiceWorker.maybeServiceWorker) {
  | None => {
    Js.log("[App] Browser does *not* support service workers");
  }
  | Some(worker) => {
    Js.log("[App] Browser supports service workers");
    open ServiceWorker;
    Js.Promise.(worker->registerOnLoad("demo-sw.js")
      |> then_((b:ServiceWorker.Registration.t) => {
        Js.log("[App] ServiceWorker registration successful with scope: " ++ b##scope);
        let c = Js.Nullable.toOption(worker##controller);
        switch(c){
        | Some(controller) => controller##onerror #= {(msg) =>Js.log2("error", msg)}
        | None => ()
        }
        resolve(Some(b));
      })
      |> catch(e => {
        Js.log2("[App] ServiceWorker registration failed: ", e);
        resolve(None)
      })
    ) |> ignore;
  }
};