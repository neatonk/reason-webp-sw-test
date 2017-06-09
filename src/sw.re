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
  external respondWith : Response.t => unit = "" [@@bs.send.pipe : t];
};

type self;

external self : self = "" [@@bs.val];

external onfetch : self => (FetchEvent.t => unit) => unit = "" [@@bs.set];

external fetch : string => Js.t {. mode : string} => Response.t = "" [@@bs.val];

let jpeg_ext_re = [%bs.re "/\\.jpe?g$/"];

onfetch
  self
  (
    fun evt => {
      let req = FetchEvent.request evt;
      let url = Request.url req;
      if (Js.Re.test url jpeg_ext_re && req |> Request.accepts "webp") {
        evt |>
        FetchEvent.respondWith (
          fetch (Js.String.replaceByRe jpeg_ext_re ".webp" url) {"mode": "no-cors"}
        )
      }
    }
  );