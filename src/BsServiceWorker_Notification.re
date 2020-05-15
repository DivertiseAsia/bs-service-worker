module Permission {
  [@bs.deriving jsConverter]
  type t = [
    | [@bs.as "default"] `default
    | [@bs.as "denied"] `denied
    | [@bs.as "granted"] `granted ];
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

[@bs.send] external requestPermission: (controller) => Js.Promise.t(Permission.t) = "requestPermission";
[@bs.new] external createNotification : string => unit = "Notification";
