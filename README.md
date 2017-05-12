# ReasonML WebP Service Worker Test

This repo contains a test project implementing a service worker which intercepts
image requests and returns WebP images where supported. This project was created
to test the use of ReasonML to create a service worker script.

## Roadmap

- Implement a P.O.C. service worker (in js) which intercepts image requests and
  responds with a WebP image where supported.
- Translate the P.O.C service worker impl. to reason.
- Download and cache a full-sized WebP image, resize according to the requested,
  and respond with the result.
- Partially download and cache the full-sized WebP image, resize and respond.
- Resume cached partial response and update the cache as needed.
- Optimize the request/resize/respond code path to minimize [TTFB][].

## License

Distributed under the ISC license.

See [LICENSE][].

[TTFB]: https://en.wikipedia.org/wiki/Time_To_First_Byte
[LICENSE]: LICENSE
