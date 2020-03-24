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
