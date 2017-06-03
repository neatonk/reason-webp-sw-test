[@@@bs.config {no_export: no_export}];

module Header = {
  type t;
  external includes : string => Js.boolean = "" [@@bs.send.pipe : t];
};

module Headers = {
  type t;
  /* external get : string => option Header.t = "" [@@bs.send.pipe : t]; */
  /* XXX TODO: Try bspack... This is more correct, but add `require` to output */
  external get : string => option Header.t =
    "" [@@bs.send.pipe : t] [@@bs.return null_undefined_to_opt];
};

module Request = {
  type t;
  external headers : t => Headers.t = "" [@@bs.get];
  external url : t => string = "" [@@bs.get];
  /* Helpers */
  let get_header name req => req |> headers |> Headers.get name;
  let accepts mime req =>
    switch (req |> get_header "accept") {
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

let is_jpeg req => Js.Re.test (Request.url req) [%bs.re "/\\.jpe?g$/"];

let replace_ext url ext => {
  let idx = Js.String.lastIndexOf "." url;
  Js.String.substring from::0 to_::idx url ^ "." ^ ext
};

onfetch
  self
  (
    fun evt => {
      let req = FetchEvent.request evt;
      if (is_jpeg req && req |> Request.accepts "webp") {
        let url = replace_ext (Request.url req) "webp";
        evt |> FetchEvent.respondWith (fetch url {"mode": "no-cors"})
      }
    }
  );
