module Subscription {
  type options = Js.t({.
    userVisibleOnly: bool,
    applicationServerKey: Js_typed_array.ArrayBuffer.t,
  });
  type t = Js.t({
    .
    endpoint: string,
    expirationTime: float, /* this is a double... may have issue */
    options: options,
    [@bs.meth] unsubscribe: unit => Js.Promise.t(bool),
    [@bs.meth] getKey: unit => Js_typed_array.ArrayBuffer.t,
  });
};

type t = Js.t({
  .
  [@bs.meth] permissionState: Subscription.options => Js.Promise.t(string),
  [@bs.meth] subscribe: Subscription.options => Js.Promise.t(Subscription.t),
});

[@bs.scope "PushManager"] [@bs.val] external supportedContentEncodings: option(array(string)) = "supportedContentEncodings";