var Jimp = require("jimp");
var fs = require("fs");
var replaceExt = require('replace-ext');

// print process.argv
// process.argv.forEach(function (val, index, array) {
//   console.log(index + ': ' + val);
// });

function ToRgb16( r, g, b) {
	r=r&0xf8;
	g=g&0xfc;
	b=b&0xf8;
	return (r << 8) | (b << 3) | (g>>2);
};

Jimp.read(process.argv[2], function (err, image) {
	
	var raw={};
	var img=[];
	var baseIndex=process.argv[3];

	image.scan(0, 0, image.bitmap.width, image.bitmap.height, function (x, y, idx) {
		// x, y is the position of this pixel on the image
		var b = this.bitmap.data[ idx + 0 ];
		var g = this.bitmap.data[ idx + 1 ];
		var r = this.bitmap.data[ idx + 2 ];

		var c=ToRgb16(r,g,b);

		if (raw[c]==undefined)
			raw[c]=baseIndex++;

		img.push(raw[c]);
	});

	fs.writeFileSync(replaceExt(process.argv[2], '.raw'),Buffer.from(img),'binary');

	var buf=new Buffer(4*(baseIndex-process.argv[3]));
	var offset=0;
	for(var colorIndex in raw){
		buf.writeUInt16BE(raw[colorIndex],offset);
		offset+=2;
		buf.writeUInt16BE(colorIndex,offset);
		offset+=2;
	}

	fs.writeFileSync(replaceExt(process.argv[2], '.clut'),buf,'binary');
});