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
  external create : string => t = "Request" [@@bs.new];
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
  external clone : t = "" [@@bs.send.pipe : t];
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

module Cache = {
  type t;
  external put : Request.t => Response.t => Js.Promise.t unit =
    "" [@@bs.send.pipe : t];
};

module CacheStorage = {
  type t;
  external openCache : string => Js.Promise.t Cache.t =
    "open" [@@bs.send.pipe : t];
  external matchReq :
    Request.t => Js.Promise.t (Js.Null_undefined.t Response.t) =
    "match" [@@bs.send.pipe : t];
};

type self;

external self : self = "" [@@bs.val];

external onfetch : self => (FetchEvent.t => unit) => unit = "" [@@bs.set];

external caches : CacheStorage.t = "" [@@bs.val];

let cache_name = "v1";

let cache_response req res => {
  let resClone = res |> Response.clone;
  /* XXX TODO check response.ok */
  Js.Promise.(
    caches |> CacheStorage.openCache cache_name |>
    then_ (Cache.put req resClone) |>
    then_ (fun _ => resolve res)
  )
};

let jpeg_ext_re = [%bs.re "/\\.jpe?g$/"];

let respond req => {
  let url = Request.url req;
  if (Js.Re.test url jpeg_ext_re && req |> Request.accepts "webp") {
    let new_url = Js.String.replaceByRe jpeg_ext_re ".webp" url;
    fetchWithInit new_url {"mode": "no-cors"} |>
    Js.Promise.then_ (cache_response (Request.create new_url))
  } else {
    fetchWithRequest req
  }
};

let cache_and_respond req =>
  respond req |> Js.Promise.then_ (cache_response req);

let log_error_and_respond req err => {
  Js.log err;
  fetchWithRequest req
};

let app req =>
  Js.Promise.(
    caches |> CacheStorage.matchReq req |>
    then_ (
      fun x =>
        switch (Js.Null_undefined.to_opt x) {
        | Some res => resolve res
        | None => cache_and_respond req
        }
    ) |>
    catch (log_error_and_respond req)
  );

onfetch
  self
  (
    fun evt => evt |> FetchEvent.respondWith (evt |> FetchEvent.request |> app)
  );
