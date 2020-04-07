[@bs.deriving abstract]
type t = {
  title: string,
};
let make = t;

module Options {
  type t;
};

module Permission : {
  type t;
  let default:t;
  let denied:t;
  let granted:t;
} = {
    type t = string;
    let default:t = "default"
    let denied:t = "denied"
    let granted:t = "granted"
};

module Direction : {
  type t;
  let auto:t;
  let ltr:t;
  let rtl:t;
} = {
    type t = string;
    let auto:t = "auto"
    let ltr:t = "ltr"
    let rtl:t = "rtl"
};

[@bs.scope "window"] [@bs.val] external maybeNotification: option(t) = "Notification";

[@bs.send] external permission: (t) => Permission.t = "permission";
[@bs.send] external requestPermission: (t) => Js.Promise.t(Permission.t) = "requestPermission";

