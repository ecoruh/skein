var assert = require('assert');
var skein = require('./build/Release/skein');
assert.equal('skein', skein.name());
console.log('skein.name() =', skein.name());