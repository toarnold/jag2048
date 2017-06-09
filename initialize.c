#include <jaglib.h>
#include <jagcore.h>
#include "grid.h"
#include "globals.h"
#include "imgobjects.h"

op_bmp_object_scaled *scaledBmp;
op_bmp_object *gameoverbmp;
op_bmp_object *youwinbmp;
op_branch_object *tilesbranch;
uint64_t *listbufshadow;
uint64_t *listbuf;

void _INIT_2048()
{
	uint16_t index = 0;
	jag_set_indexed_color(JAG_CONSOLE_BACKGROUND_COLORINDEX,toRgb16(0, 0, 0));
	jag_set_indexed_color(JAG_CONSOLE_STDOUT_COLORINDEX,toRgb16(0xff, 0xff, 0xff));
	jag_set_indexed_color(0,toRgb16(0, 0, 0));
	jag_set_indexed_color(1,toRgb16(0xff, 0xff, 0xff)); // White number in tiles
	jag_set_indexed_color(2,toRgb16(0x77, 0x6e, 0x65)); // dark-Grey?
	jag_set_indexed_color(3,toRgb16(192, 192, 192));
	jag_set_indexed_color(4,toRgb16(160, 160, 160)); // 2=eee4da
	jag_set_indexed_color(5,toRgb16(128, 128, 128)); // 4=ede0c8
	jag_set_indexed_color(6,toRgb16(0xf2, 0xb1, 0x79)); // 8=f2b179
	jag_set_indexed_color(7,toRgb16(0xf5, 0x95, 0x63)); // 16=f59563
	jag_set_indexed_color(8,toRgb16(0xf6, 0x7c, 0x5f)); // 32=f67c5f
	jag_set_indexed_color(9,toRgb16(0xf6, 0x5e, 0x3b)); // 64=f65e3b
	jag_set_indexed_color(10,toRgb16(0xed, 0xcf, 0x72)); // 128=edcf72
	jag_set_indexed_color(11,toRgb16(0xed, 0xcc, 0x61)); // 256=edcc61
	jag_set_indexed_color(12,toRgb16(0xed, 0xc8, 0x50)); // 512=edc850
	jag_set_indexed_color(13,toRgb16(0xed, 0xc53, 0xf)); // 1024=edc53f
	jag_set_indexed_color(14,toRgb16(0xed, 0xc2, 0x2e)); // 2048=edc22e
	jag_set_indexed_color(15,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32
	jag_set_indexed_color(16,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32
	jag_set_indexed_color(17,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32
	jag_set_indexed_color(18,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32
	jag_set_indexed_color(19,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32
	jag_set_indexed_color(20,toRgb16(0x3c, 0x3a, 0x32)); // MAX=3c3a32

	while (P2048title_clut[index])
	{
		jag_set_indexed_color(P2048title_clut[index],P2048title_clut[index+1]);
		index += 2;
	}

	listbuf=calloc(8, 41);
	listbufshadow=calloc(8, 41);
	uint32_t base = (uint32_t)listbuf >> 3;

	// Create 16 bmp Blocks
	GridCreate(jag_console_bmp, base+2);

	// 32 scaled bmp for effect
	scaledBmp = (op_bmp_object_scaled *)&listbufshadow[32];
	scaledBmp->p0.type = SCBITOBJ;
	scaledBmp->p0.ypos = 0;
	scaledBmp->p0.height = 40-1;
	scaledBmp->p0.link = base + 36;
	scaledBmp->p0.data = (uint32_t)block2048 >> 3;
	scaledBmp->p1.xpos = 400;
	scaledBmp->p1.depth = O_DEPTH8 >> 12;
	scaledBmp->p1.pitch = 1;
	scaledBmp->p1.dwidth = 40 / 8;
	scaledBmp->p1.iwidth = 40 / 8;
	scaledBmp->p1.trans = 1;
	scaledBmp->p2.hscale = 1 << 4; // 1
	scaledBmp->p2.vscale = 1 << 5; // 1
	scaledBmp->p2.remainder = 1<<4; // 0.5

	// GameOver
	gameoverbmp = (op_bmp_object *)&listbufshadow[36];
	gameoverbmp->p0.type = BITOBJ;
	gameoverbmp->p0.ypos = jag_console_bmp->p0.ypos + 184;
	gameoverbmp->p0.height = 16;
	gameoverbmp->p0.link = base + 38;
	gameoverbmp->p0.data = (uint32_t)gameover >> 3;
	gameoverbmp->p1.xpos = -200; //bmp->p1.xpos + 80;
	gameoverbmp->p1.depth = O_DEPTH8 >> 12;
	gameoverbmp->p1.pitch = 1;
	gameoverbmp->p1.dwidth = 160 / 8;
	gameoverbmp->p1.iwidth = 160 / 8;
	gameoverbmp->p1.trans = 1;

	// Youwin
	youwinbmp = (op_bmp_object *)&listbufshadow[38];
	youwinbmp->p0.type = BITOBJ;
	youwinbmp->p0.ypos = jag_console_bmp->p0.ypos + 176;
	youwinbmp->p0.height = 31;
	youwinbmp->p0.link = base + 40;
	youwinbmp->p0.data = (uint32_t)youwin >> 3;
	youwinbmp->p1.xpos = -200; // bmp->p1.xpos + 96;
	youwinbmp->p1.depth = O_DEPTH8 >> 12;
	youwinbmp->p1.pitch = 1;
	youwinbmp->p1.dwidth = 128 / 8;
	youwinbmp->p1.iwidth = 128 / 8;
	youwinbmp->p1.trans = 1;

	// 40 Stop object
	op_stop_object *stop = (op_stop_object *)&listbufshadow[40];
	stop->type = STOPOBJ;
	stop->int_flag = 1;

	uint64_t *branch=calloc(2,8);

	tilesbranch=(op_branch_object *)branch;
	tilesbranch->type=BRANCHOBJ;
	tilesbranch->ypos=0x3ff;
	tilesbranch->cc=2; // branch never
	tilesbranch->link=base; // Here starts the tiles list

	stop=(op_stop_object *)&branch[1];
	stop->type = STOPOBJ;
	stop->int_flag = 1;

	jag_append_olp(branch);
}
