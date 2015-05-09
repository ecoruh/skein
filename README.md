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

### hash
The method `hash` takes a string argument and returns a 512-bit (64-byte) hash value in a Buffer object produced from it.

```javascript
 var crypto = new skein.Crypto();
 var hash = crypto.hash(“my top secret password”);
 assert.equal(hash.length, 64);
 ```

### echo
The method `echo` takes a Buffer argument and returns a copy of it.

```javascript
var obj = new skein.Crypto();
var expected = new Buffer([1, 2, 3]);
var actual = obj.echo(expected);
assert.ok(actual.compare(expected) === 0);
```

## Acknowledgement
Skein was developed by Niels Ferguson, Stefan Lucks, Bruce Schneier, Doug Whiting, Mihir Bellare, Tadayoshi Kohno, Jon Callas, Jesse Walker. 

## Reference
[1 Skein website](https://www.schneier.com/skein.html)
