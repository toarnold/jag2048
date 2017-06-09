var Jimp = require("jimp");
var fs = require("fs");
var replaceExt = require('replace-ext');

//print process.argv
// process.argv.forEach(function (val, index, array) {
//    console.log(index + ': ' + val);
// });

Jimp.read(process.argv[2], function (err, image) {
	
	var raw=[];

	image.scan(0, 0, image.bitmap.width, image.bitmap.height, function (x, y) {
		// x, y is the position of this pixel on the image

		var color = image.getPixelColor(x, y);

		switch(color){
			case 4278190335: // edge
				raw.push(0);
				break;
			case 8388863: // number
				raw.push(1);
				break;
			case 255:
			case 0:
				raw.push(process.argv[3]);
				break;
			default: // unkown color
				console.log(color);
				break;
		}
	});

	fs.writeFileSync(replaceExt(process.argv[2], '.raw'),Buffer.from(raw),'binary');
});