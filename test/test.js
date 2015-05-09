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

describe('CalcHash test', function () {
   describe('calcHash test', function () {

      it('should calculate a hash', function () {
         var crypto = new skein.Crypto();
         var hash = crypto.calcHash("my top secret password");
         assert.equal(hash.length, 64);
      });

      it('should throw wrong number of arguments exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.calcHash();
         } catch (e) {
            assert.equal(e.message, 'There must be one String argument for password');
         }
      });

      it('should throw argument type exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.calcHash(123);
         } catch (e) {
            assert.equal(e.message, 'Argument type should be a String');
         }
      });
   });
});

describe('SetHash/GetHash test', function () {
   describe('setHash/getHash test', function () {

      it('should validate hash via set/get methods', function () {
         var crypto = new skein.Crypto();
         var hash = crypto.calcHash("stritcly boring password");
         assert.equal(hash.length, 64);
         crypto.setHash(hash);
         var actual = crypto.getHash();
         assert.ok(actual.compare(hash) === 0);
      });

      it('should throw missing argument exception for setHash', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.setHash();
         } catch (e) {
            assert.equal(e.message, 'There must be one Buffer argument for hash');
         }
      });

      it('should throw missing argument exception for setHash', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.setHash(new Buffer([2, 4, 6]));
         } catch (e) {
            assert.equal(e.message, "The Buffer argument's length must be 64 bytes");
         }
      });

      it('should throw missing argument exception for getHash', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.getHash('a');
         } catch (e) {
            assert.equal(e.message, "There must be no arguments");
         }
      });

      it('should throw hash must be set exception for getHash', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.getHash();
         } catch (e) {
            assert.equal(e.message, "The hash must be set first. Use setHash");
         }
      });

   });
});
