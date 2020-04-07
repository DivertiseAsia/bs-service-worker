switch(Notification.maybeNotification) {
  | None => {
    Js.log("[App] Browser does *not* support notifications");
  }
  | Some(notification) => {
    Js.log("[App] Browser supports notifications");
    open Notification;
    Js.Promise.(notification->requestPermission
      |> then_((p:Permission.t) => {
        Js.log2("[App] Permission request successful with answer: ", p);
        if (p == Permission.granted) {
          Js.log("[App] Permission granted");
        } else {
          Js.log("[App] Permission rejected");
        }
        resolve();
      })
      |> catch(e => {
        Js.log2("[App] Permission request failed failed: ", e);
        resolve()
      })
    ) |> ignore;
  }
};