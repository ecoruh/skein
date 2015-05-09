/*jslint node: true */
/*global describe, it */
"use strict";

var assert = require('assert');
var skein = require('../build/Release/skein');

describe('Echo test', function () {
   describe('echo test', function () {
      it('should echo buffer object', function () {
         var crypto = new skein.Crypto();
         var expected = new Buffer([1, 2, 3]);
         var actual = crypto.echo(expected);
         assert.ok(actual.compare(expected) === 0);
      });
   });
});

describe('Hash test', function () {
   describe('hash test', function () {

      it('should yield a hash', function () {
         var crypto = new skein.Crypto();
         var hash = crypto.hash("my top secret password");
         assert.equal(hash.length, 64);
      });

      it('should throw wrong number of arguments exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.hash();
         } catch (e) {
            assert.equal(e.message, 'There must be one string argument for password');   
         }
      });

      it('should throw argument type exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.hash(123);
         } catch (e) {
            assert.equal(e.message, 'Argument type should be a string');   
         }
      });
   });
});