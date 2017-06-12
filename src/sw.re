[@@@bs.config {no_export: no_export}];

module Header = {
  type t;
  external includes : string => Js.boolean = "" [@@bs.send.pipe : t];
};

module Headers = {
  type t;
  external get : string => option Header.t =
    "" [@@bs.send.pipe : t] [@@bs.return null_undefined_to_opt];
};

module RequestInit = {
  type t = Js.t {. mode : string};
};

module Request = {
  type t;
  external headers : t => Headers.t = "" [@@bs.get];
  external url : t => string = "" [@@bs.get];
  /* Helpers */
  let accepts mime req =>
    switch (req |> headers |> Headers.get "accept") {
    | None => false
    | Some hdr => Js.to_bool (Header.includes mime hdr)
    };
};

module Response = {
  type t;
};

module FetchEvent = {
  type t;
  external request : t => Request.t = "" [@@bs.get];
  external respondWith : Js.Promise.t Response.t => unit =
    "" [@@bs.send.pipe : t];
};

external fetch : string => Js.Promise.t Response.t = "fetch" [@@bs.val];

external fetchWithInit : string => RequestInit.t => Js.Promise.t Response.t =
  "fetch" [@@bs.val];

external fetchWithRequest : Request.t => Js.Promise.t Response.t =
  "fetch" [@@bs.val];

external fetchWithRequestInit :
  Request.t => RequestInit.t => Js.Promise.t Response.t =
  "fetch" [@@bs.val];

module Caches = {
  type t;
  external matchReq :
    Request.t => Js.Promise.t (Js.Null_undefined.t Response.t) =
    "match" [@@bs.send.pipe : t];
};

type self;

external self : self = "" [@@bs.val];

external onfetch : self => (FetchEvent.t => unit) => unit = "" [@@bs.set];

external caches : Caches.t = "" [@@bs.val];

let jpeg_ext_re = [%bs.re "/\\.jpe?g$/"];

let response req => {
  let url = Request.url req;
  caches |> Caches.matchReq req |>
  Js.Promise.then_ (
    fun x =>
      switch (Js.Null_undefined.to_opt x) {
      | Some res => Js.Promise.resolve res
      | None =>
        if (Js.Re.test url jpeg_ext_re && req |> Request.accepts "webp") {
          fetchWithInit
            (Js.String.replaceByRe jpeg_ext_re ".webp" url) {"mode": "no-cors"}
        } else {
          fetchWithRequest req
        }
      }
  )
};

onfetch
  self
  (
    fun evt =>
      evt |> FetchEvent.respondWith (evt |> FetchEvent.request |> response)
  );
