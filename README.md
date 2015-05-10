# skein
node.js add-on for skein hash functions 

## Status
This version of skein add-on is under development and incomplete. The functions currently implemented for OS X and Windows platforms are described below. There will be an MVP (Minimal Viable Product) soon which will be built for OS X and Windows. Linux and Raspbian support will follow. Stay tuned.

## What is Skein?
Skein is a family of cryptographic hash functions. Skein’s design combines speed, security, simplicity, and a great deal of flexibility in a modular package that is easy to analyze  [1]

## What is skein add-on?
Skein add-on is a node add-on that wraps Skein C implementation. Skein implementation is known for its speed. The objective of this project is to make Skein available for node.js packages without sacrificing performance. The wrapper will also include crypto functions leveraging high speed and security.

## How do I install?

```
npm install skein
```

## skein functions
The following functions are currently available from the skein add-on.

### calcHash
The method `calcHash` takes a string argument and returns a 512-bit (64-byte) hash value in a Buffer object. The buffer object is NOT stored internally.

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("my top secret password", function (err, data) {
   assert.equal(err, null);
   assert.equal(data.length, 64);
});
```

### setHash
The method `setHash` takes a Buffer argument with length 64 and sets the hash value on the `Crypto` object.

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("stritcly boring password", function(err, data) {
   assert.equal(data.length, 64);
   crypto.setHash(data);
   crypto.getHash( function (err, hash) {
      assert.ok(hash.compare(data) === 0);
   });
});
```

### getHash
The method `getHash` returns the current hash value of the `Crypto` object as a 64 byte Buffer object. If the object's hash value was not set previously with a `setHash` call, this function returns an error. 

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
crypto.calcHash("another boring password", function(err, data) {
   assert.equal(data.length, 64);
   crypto.setHash(data);
   crypto.getHash( function (err, hash) {
      assert.ok(hash.compare(data) === 0);
   });
});
```

### echo
The method `echo` takes a Buffer argument and returns a copy of it in a callback.

```javascript
var skein = require('skein');
var crypto = new skein.Crypto();
var expected = new Buffer([1, 2, 3]);
crypto.echo(expected, function (data) {
   assert.ok(data.compare(expected) === 0);
   done();
});
```

## Acknowledgement
Skein was developed by Niels Ferguson, Stefan Lucks, Bruce Schneier, Doug Whiting, Mihir Bellare, Tadayoshi Kohno, Jon Callas, Jesse Walker. 

## Reference
[1 Skein website](https://www.schneier.com/skein.html)
