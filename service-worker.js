"use strict";

//self.addEventListener('fetch', function(event) {
self.onfetch = function(event) {
  if ( /\.jpg$/.test(event.request.url) ) {
    let req = event.request;
    let hdr = req.headers;
    if ( hdr.has('accept') && hdr.get('accept').includes('webp') ) {
      let url = req.url.substr(0, req.url.lastIndexOf(".")) + ".webp";
      event.respondWith(fetch(url, { mode: 'no-cors' }));
    }
  }
};
