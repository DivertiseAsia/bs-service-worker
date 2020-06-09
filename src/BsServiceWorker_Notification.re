module Permission : {
  type t;
  let default:t;
  let denied:t;
  let granted:t;
  let unknow:t;
} = {
    type t = string;
    let default:t = "default"
    let denied:t = "denied"
    let granted:t = "granted"
    let unknow:t = "unknow"
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

type controller = Js.t({.
  permission: Permission.t,
});

[@bs.scope "window"] [@bs.val] external maybeNotification: option(controller) = "Notification";

[@bs.get] external permission: (controller) => Permission.t = "permission";
[@bs.send] external requestPermission: (controller) => Js.Promise.t(Permission.t) = "requestPermission";
[@bs.new] external createNotification : string => unit = "Notification";
