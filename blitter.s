	include "jaguar.inc"

	text

	xdef _fill_background
_fill_background: ; void fill_background()
	move.l	B_CMD,d0
	ror.w	#1,d0
	bcc.s	_fill_background			; Wait Blitter ready
	move.l	_jag_vidmem,A1_BASE		; dest
	move.l	#60,A1_PIXEL
	move.l	#PIXEL8 | XADDPIX | WID320 | PITCH1,A1_FLAGS
	move.l	#$0001ff38,A1_STEP
	move.l	#2,B_PATD		; pattern Palette-Color 2
	move.l	#$00c800c8,B_COUNT	; 200x200 Bytes
	move.l	#UPDA1|PATDSEL,B_CMD
	rts

	xdef _fill_backgrid
_fill_backgrid: ; void fill_backgrid(uint16_t offset)
	move.l	B_CMD,d0
	ror.w	#1,d0
	bcc.s	_fill_backgrid			; Wait Blitter ready
	move.l	_jag_vidmem,A1_BASE		; dest
	move.l	4(SP),A1_PIXEL		; offset
	move.l	#PIXEL8 | XADDPIX | WID320 | PITCH1,A1_FLAGS
	move.l	#$0001ffd8,A1_STEP
	move.l	#_block_nul,A2_BASE		; src
	move.l	#0,A2_PIXEL
	move.l	#PIXEL8 | XADDPIX | WID40 | PITCH1,A2_FLAGS
	move.l	#$0001ffd8,A2_STEP
	move.l	#$00280028,B_COUNT	; 40x40 Bytes
	move.l	#SRCEN|DSTEN| UPDA1|UPDA2|LFU_SORD,B_CMD
	rts

	xdef _show_logo
_show_logo: ; void show_logo()
	move.l	B_CMD,d0
	ror.w	#1,d0
	bcc.s	_show_logo			; Wait Blitter ready
	move.l	_jag_vidmem,A1_BASE	;destAdr
	move.l	#$60,A1_PIXEL		; offset
	move.l	#PIXEL8 | XADDPIX | WID320 | PITCH1,A1_FLAGS
	move.l	#$0001FF80,A1_STEP
	move.l	#_P2048title_data,A2_BASE		; src
	move.l	#0,A2_PIXEL
	move.l	#$0001FF80,A2_STEP
	move.l	#PIXEL8 | XADDPIX | WID128 |PITCH1,A2_FLAGS
	move.l	#$00800080,B_COUNT
	move.l	#SRCEN|DSTEN| UPDA1|UPDA2|LFU_REPLACE,B_CMD
	rts
