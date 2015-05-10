/*jslint node: true */
"use strict";

var assert = require('assert');
var skein = require('./build/Release/skein');

var buf = new Buffer([1, 2, 3]);
var obj = new skein.Crypto();

obj.echo(buf, function (data) {
   console.log(data);
});

