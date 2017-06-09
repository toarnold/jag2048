	xdef _P2048title_clut
	xdef _P2048title_data
	xdef _block2
	xdef _block4
	xdef _block8
	xdef _block16
	xdef _block32
	xdef _block64
	xdef _block128
	xdef _block256
	xdef _block512
	xdef _block1024
	xdef _block2048
	xdef _block4096
	xdef _block8192
	xdef _block16384
	xdef _block32768
	xdef _block65536
	xdef _block131072
	xdef _block_nul
	xdef _gameover
	xdef _youwin

	data
	cnop 0,2
_P2048title_clut:
	incbin "images/2048title.clut"
	dc.w 0,0

	cnop 0,8
_P2048title_data:
	incbin "images/2048title.raw"
	cnop 0,8
_block2:	incbin "tiles/2.raw"
	cnop 0,8
_block4:	incbin "tiles/4.raw"
	cnop 0,8
_block8:	incbin "tiles/8.raw"
	cnop 0,8
_block16:	incbin "tiles/16.raw"
	cnop 0,8
_block32:	incbin "tiles/32.raw"
	cnop 0,8
_block64:	incbin "tiles/64.raw"
	cnop 0,8
_block128:	incbin "tiles/128.raw"
	cnop 0,8
_block256:	incbin "tiles/256.raw"
	cnop 0,8
_block512:	incbin "tiles/512.raw"
	cnop 0,8
_block1024:	incbin "tiles/1024.raw"
	cnop 0,8
_block2048:	incbin "tiles/2048.raw"
	cnop 0,8
_block4096:	incbin "tiles/4096.raw"
	cnop 0,8
_block8192:	incbin "tiles/8192.raw"
	cnop 0,8
_block16384:	incbin "tiles/16384.raw"
	cnop 0,8
_block32768:	incbin "tiles/32768.raw"
	cnop 0,8
_block65536:	incbin "tiles/65536.raw"
	cnop 0,8
_block131072:	incbin "tiles/131072.raw"
	cnop 0,8
_block_nul:	incbin "tiles/empty.raw"
	cnop 0,8
_gameover:
	incbin "images/gameover.raw"
	cnop 0,8
_youwin:
	incbin "images/youwin.raw"
