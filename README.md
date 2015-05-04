# skein
node.js add-on for skein hash functions 

## Acknowledgement
Skein was developed by Niels Ferguson, Stefan Lucks, Bruce Schneier, Doug Whiting, Mihir Bellare, Tadayoshi Kohno, Jon Callas, Jesse Walker. 

## What is Skein?
Skein is a family of cryptographic hash functions. Skein’s design combines speed, security, simplicity, and a great deal of flexibility in a modular package that is easy to analyze  [1]

## What is skein add-on?
Skein add-on is a node add-on that wraps Skein C implementation. Skein implementation is known for its speed. The objective of this project is to make Skein available for node.js packages without sacrificing performance. The wrapper will also include crypto functions leveraging high speed and security.

##How do I install?

```
npm install skein
```

## skein functions
The following functions are available from the skein add-on.

### name
The method `name` returns the string `skein`.

```javascript
var assert = require('assert');
var skein = require('skein');
assert.equal('skein', skein.name());
```

## Release Notes
This version of skein add-on is under development and not stable.

## Reference
[1 Skein website](https://www.schneier.com/skein.html)
