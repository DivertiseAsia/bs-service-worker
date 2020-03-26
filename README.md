# bs-service-worker
Buckle script `service-worker` binding

# Installation

1. `npm install @divertiseasia/bs-service-worker --save`
2. Add `@divertiseasia/bs-service-worker` to bsconfig.json
3. Enjoy!

# Example

```
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
```
