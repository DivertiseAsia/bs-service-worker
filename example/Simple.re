switch(ServiceWorker.maybeServiceWorker){
| None => Js.log("[App] No ServiceWorker")
| Some(worker) =>{
    open ServiceWorker;
    Window.addEventListener("load", () => {
      Js.Promise.(worker->register("demo-sw.js")
        |> then_((b:ServiceWorker.Registration.t) => {
          Js.log2("[App] ServiceWorker registration success: ", b##scope);
          resolve(Some(b));
        })
        |> catch(e => {
          Js.log2("[App] ServiceWorker registration failed (expected): ", e);
          resolve(None)
        })
      ) |> ignore;
    })
  }
}