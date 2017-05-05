# ReasonML WebP Service Worker Test

This repo contains a test project implementing a service worker which intercepts
image requests and returns WebP images where supported. This project was created
to test the use of ReasonML to create a service worker script.

## Roadmap

- Implement a minimal service worker (in js) which intercepts image requests,
  calls a no-op function, and responds width the result.
- Translate the minimla service worker impl. to reason OR define a simple
  interface which can be used more easily used from reason code.
- Replace the no-op function with a reason function which responds with a WebP
  image where supported.
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
