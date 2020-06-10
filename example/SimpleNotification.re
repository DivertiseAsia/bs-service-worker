open BsServiceWorker;
switch(Notification.maybeNotification) {
  | None => {
    Js.log("[App] Browser does *not* support notifications");
  }
  | Some(notification) => {
    Js.log("[App] Browser supports notifications");
    open Notification;
    Js.Promise.(notification->requestPermission
      |> then_((p) => {
        Js.log2("[App] Permission request successful with answer: ", p);
        switch (p |> mapStrPermissionType) {
        | Granted =>
         { Js.log("[App] Permission granted");
          createNotification("Test notification!")}
        | _ => Js.log("[App] Permission rejected");
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