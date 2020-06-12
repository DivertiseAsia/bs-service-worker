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

type permissionType = 
  | Default
  | Denied
  | Granted
  | Unknow;

let mapStrPermissionType = permission: permissionType =>
  switch (permission) {
  | "default" => Default
  | "denied" => Denied
  | "granted" => Granted
  | _ => Unknow
  };

type controller = Js.t({.
  permission: string,
});

[@bs.scope "window"] [@bs.val] external maybeNotification: option(controller) = "Notification";

[@bs.get] external permission: (controller) => string = "permission";
[@bs.send] external requestPermission: (controller) => Js.Promise.t(string) = "requestPermission";
[@bs.new] external createNotification : string => unit = "Notification";
