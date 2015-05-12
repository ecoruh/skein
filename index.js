var skein = null;
 
// Load the precompiled binary for windows.
if(process.platform == "win32") {
   skein = require('./bin/win/skein');
} else if(process.platform == "darwin") {
   skein = require('./bin/osx/skein');
} else if(process.platform == 'linux') {
   skein = require('./bin/linux/skein');   
}
 
module.exports = skein;
	