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

describe('Encrypt test', function () {
   describe('encrypt test', function () {

      it('should encrypt a string', function (done) {
         var crypto = new skein.Crypto();
         crypto.calcHash("my top secret password", function (err, hash) {
            assert.equal(err, null);
            assert.equal(hash.length, 64);
            crypto.encrypt(hash, "Οὐχὶ ταὐτὰ παρίσταταί μοι γιγνώσκειν", function (err, data) {
               assert.equal(err, null);
               assert.equal(data.length, 76);
               done();
            });
         });
      });

      it('should throw wrong number of arguments exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.encrypt();
         } catch (e) {
            assert.equal(e.message, 'There must be 3 arguments');
         }
      });

      it('should throw argument type exception', function () {
         var crypto = new skein.Crypto();
         try {
            crypto.encrypt(123, "foo", 123);
         } catch (e) {
            assert.equal(e.message, 'Expected a buffer, a string and a function');
         }
      });


   });
});

describe('Decrypt test', function () {
   describe('decrypt test', function () {

      it('should decrypt a string', function (done) {
         var crypto = new skein.Crypto();
         crypto.calcHash("my top secret password", function (err, hash) {
            assert.equal(err, null);
            assert.equal(hash.length, 64);
            var text = "Οὐχὶ ταὐτὰ παρίσταταί μοι γιγνώσκειν";
            crypto.encrypt(hash, text, function (err, cyphered) {
               assert.equal(err, null);
               crypto.decrypt(hash, cyphered, function (err, decyphered) {
                  assert.equal(err, null);
                  assert.equal(decyphered, text);
                  done();
               });
            });
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

