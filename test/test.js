/*jslint node: true */
/*global describe, it */
"use strict";

var assert = require('assert');
var skein = require('../build/Release/skein');

describe('Echo test', function () {
   describe('echo test', function () {
      it('should echo buffer object', function () {
         var obj = new skein.Crypto();
         var expected = new Buffer([1, 2, 3]);
         var actual = obj.echo(expected);
         assert.ok(actual.compare(expected) === 0);
      });
   });
});