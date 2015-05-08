var assert = require('assert');
var skein = require('skein');

var buf = new Buffer([1, 2, 3]);
var obj = new skein.Crypto();
console.log(obj.echo(buf));