/*jslint node: true */
/*global describe, it */
"use strict";

var assert = require('assert');
var skein = require('../build/Release/skein');

describe('Echo test', function () {
   describe('echo test', function () {
      it('should echo buffer object', function (done) {
         var crypto = new skein.Crypto();
         var expected = new Buffer([1, 2, 3]);
         crypto.echo(expected, function (data) {
            assert.ok(data.compare(expected) === 0);
            done();
         });
      });
   });
});

describe('CalcHash test', function () {
   describe('calcHash test', function () {
      it('should calculate a hash', function (done) {
         var crypto = new skein.Crypto();
         crypto.calcHash("my top secret password", function (err, data) {
            assert.equal(err, null);
            assert.equal(data.length, 64);
            done();
         });
      });

      it('should throw wrong number of arguments exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.calcHash();
         } catch (e) {
            assert.equal(e.message, 'There must be two arguments');
         }
      });

      it('should throw argument type exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.calcHash(123, "foo");
         } catch (e) {
            assert.equal(e.message, 'Expected a string and a function');
         }
      });
   });
});

describe('SetHash/GetHash test', function () {
   describe('setHash/getHash test', function () {

      it('should validate hash via set/get methods', function (done) {
         var crypto = new skein.Crypto();
         crypto.calcHash("stritcly boring password", function(err, data) {
            assert.equal(data.length, 64);
            crypto.setHash(data);
            crypto.getHash( function (err, hash) {
               assert.ok(hash.compare(data) === 0);
               done();               
            });
         });
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
            assert.equal(e.message, "There must be one function argument");
         }
      });

      it('should throw hash must be set exception for getHash', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.getHash();
         } catch (e) {
            assert.equal(e.message, "There must be one function argument");
         }
      });

   });
});
