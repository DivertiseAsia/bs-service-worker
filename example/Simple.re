open DivertiseasiaBsServiceWorker;
if (ServiceWorker.isSupported()) {
  ServiceWorker.windowAddEventListener("load", () => {
    Js.Promise.(ServiceWorker.register("demo-sw.js")
      |> then_((b:ServiceWorker.ServiceWorkerRegistration.t) => {
        Js.log2("[App] ServiceWorker registration success: ", b#scope);
        resolve(Some(b));
      })
      |> catch(e => {
        Js.log2("[App] ServiceWorker registration failed (expected): ", e);
        resolve(None)
      })
    ) |> ignore;
  })
}