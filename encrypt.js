/*jslint node: true */
"use strict";

var fs = require('fs');
var skein = require('skein');

var crypto = new skein.Crypto();

if (process.argv.length != 5) {

  console.log('node encrypt input_file output_file password');
  process.exit(1);

} else {

  var input_file = process.argv[2];
  var output_file = process.argv[3];
  var password = process.argv[4];

  fs.readFile(input_file, function (err, data) {
    if (err) throw err;    
    
    crypto.calcHash(password, function (err, hash) {
      if (err) throw err;    
      
      crypto.encrypt(hash, data.toString(), function (err, encrypted) {
        if (err) throw err;    
        var wstream = fs.createWriteStream(output_file);
        wstream.write(encrypted);       
        console.log('done!');
      });
    });    
  });
}



