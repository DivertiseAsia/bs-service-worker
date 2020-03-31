# bs-service-worker
Buckle script `service-worker` binding

## Status
* Master ![build](https://github.com/DivertiseAsia/bs-service-worker/workflows/build/badge.svg?branch=master)
* Release ![npm-publish](https://github.com/DivertiseAsia/bs-service-worker/workflows/npm-publish/badge.svg?branch=release)

# Installation

1. `npm install @divertiseasia/bs-service-worker --save`
2. Add `@divertiseasia/bs-service-worker` to bsconfig.json
3. Enjoy!

# Example

```
open DivertiseasiaBsServiceWorker;
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
        resolve(Some(b));
      })
      |> catch(e => {
        Js.log2("[App] ServiceWorker registration failed: ", e);
        resolve(None)
      })
    ) |> ignore;
  }
};
```

More examples can be found at [@divertiseasia/bs-service-worker-examples](https://github.com/DivertiseAsia/bs-service-worker-examples)
