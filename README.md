# skein
node.js encryption add-on using Skein hash functions 

## What is Skein?
Skein is a family of cryptographic hash functions. Skein’s design combines speed, security, simplicity, and a great deal of flexibility in a modular package that is easy to analyze.  [1]

## What is skein add-on?
Skein add-on wraps original Skein C implementation. The add-on makes Skein available for node.js packages without sacrificing its speed. The wrapper includes a 512-bit hash function and a pair of 512-bit symmetric encryption functions.

## Status
The add-on is currently available for Linux, OS X and Windows platforms. Raspbian build is in progress and it will be available soon. Please stay tuned and check out this page for further news.

## How do I install?

```
npm install skein
```

## skein functions
The following functions are currently available from the skein add-on. These functions are asynchronous in nature and conforms to node.js 'error-first' callback convention. Argument errors can cause exceptions to be thrown.

### calcHash
The method `calcHash` takes a string argument and returns a 512-bit (64-byte) hash value in a `Buffer` object. The `Buffer` object is NOT stored internally. The `Buffer` object can be used as a key in the `encrypt` and `decrypt` functions.

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("my top secret password", function (err, hash) {
   assert.equal(err, null);
   assert.equal(hash.length, 64);
});
```
### encrypt
The method `encrypt` takes a 64-byte hash value as a key, a `String` object to encrypt, and a function object to return encrypted binary data in a `Buffer` object. Typically the hash value is calculated using the `calcHash` function although it does not have to be.

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("my top secret password", function (err, hash) {
   assert.equal(err, null);
   assert.equal(hash.length, 64);
   crypto.encrypt(hash, "Οὐχὶ ταὐτὰ παρίσταταί μοι γιγνώσκειν", function (err, encrypted) {
      assert.equal(err, null);
      assert.equal(encrypted.length, 76);
      done();
   });
});
```

### decrypt
The method `decrypt` takes a 64-byte hash value as a key, a binary `Buffer` object to decrypt, and a function object to return decrypted data in a `String` object. Typically the hash value is calculated using the `calcHash` function although it does not have to be. 

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("my top secret password", function (err, hash) {
   assert.equal(err, null);
   assert.equal(hash.length, 64);
   var text = "Οὐχὶ ταὐτὰ παρίσταταί μοι γιγνώσκειν";
   crypto.encrypt(hash, text, function (err, encrypted) {
      assert.equal(err, null);
      crypto.decrypt(hash, encrypted, function (err, decrypted) {
         assert.equal(err, null);
         assert.equal(decrypted, text);
         done();
      });
   });
});
 ```

## Acknowledgement
Skein was developed by Niels Ferguson, Stefan Lucks, Bruce Schneier, Doug Whiting, Mihir Bellare, Tadayoshi Kohno, Jon Callas, Jesse Walker. 

## Reference
[1 Skein website](https://www.schneier.com/skein.html)
