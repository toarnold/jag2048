BINFILE = jag2048.jag
OBJPATH = obj/
C2OBJ = vc +jaguar -O2 -c -c99 -o $@ $?
ASM2OBJ = vc +jaguar $? -c -o $@
TILE2RAW = node tools/tile2raw $?
IMG2RAW = node tools/img2raw $?
PROJECT = $(OBJPATH)main.o $(OBJPATH)imgobjects.o $(OBJPATH)blitter.o $(OBJPATH)grid.o $(OBJPATH)screens.o $(OBJPATH)initialize.o 
TILES =  tiles/empty.raw tiles/2.raw tiles/4.raw tiles/8.raw tiles/16.raw tiles/32.raw tiles/64.raw tiles/128.raw tiles/256.raw tiles/512.raw \
	tiles/1024.raw tiles/2048.raw tiles/4096.raw tiles/8192.raw tiles/16384.raw tiles/32768.raw tiles/65536.raw tiles/131072.raw \
	images/youwin.raw images/gameover.raw images/2048title.raw

all: build

runvj: build uploadvj

runskunk: build upload

build: pre $(TILES) $(PROJECT)
	vc +jaguar -o $(BINFILE) $(PROJECT)

pre:
	mkdir -p $(OBJPATH)
	if [ ! -d "node_modules" ]; then npm install; fi

upload:
	-jcp -r
	sleep 4
	jcp $(BINFILE)

uploadvj:
	start virtualjaguar $(BINFILE)

clean:
	rm $(BINFILE) -f
	rm $(OBJPATH) -rf
	rm tiles/*.raw -f
	rm images/*.raw -f
	rm images/*.clut -f

# Default rules
$(OBJPATH)%.o: %.c
	$(C2OBJ)
$(OBJPATH)%.o: %.s
	$(ASM2OBJ)

# Images
tiles/empty.raw: tiles/empty.bmp
	$(TILE2RAW) 3
tiles/2.raw: tiles/2.bmp
	$(TILE2RAW) 4
tiles/4.raw: tiles/4.bmp
	$(TILE2RAW) 5
tiles/8.raw: tiles/8.bmp
	$(TILE2RAW) 6
tiles/16.raw: tiles/16.bmp
	$(TILE2RAW) 7
tiles/32.raw: tiles/32.bmp
	$(TILE2RAW) 8
tiles/64.raw: tiles/64.bmp
	$(TILE2RAW) 9
tiles/128.raw: tiles/128.bmp
	$(TILE2RAW) 10
tiles/256.raw: tiles/256.bmp
	$(TILE2RAW) 11
tiles/512.raw: tiles/512.bmp
	$(TILE2RAW) 12
tiles/1024.raw: tiles/1024.bmp
	$(TILE2RAW) 13
tiles/2048.raw: tiles/2048.bmp
	$(TILE2RAW) 14
tiles/4096.raw: tiles/4096.bmp
	$(TILE2RAW) 15
tiles/8192.raw: tiles/8192.bmp
	$(TILE2RAW) 15
tiles/16384.raw: tiles/16384.bmp
	$(TILE2RAW) 15
tiles/32768.raw: tiles/32768.bmp
	$(TILE2RAW) 15
tiles/65536.raw: tiles/65536.bmp
	$(TILE2RAW) 15
tiles/131072.raw: tiles/131072.bmp
	$(TILE2RAW) 15
images/youwin.raw: images/youwin.png
	$(TILE2RAW) 2
images/gameover.raw: images/gameover.png
	$(TILE2RAW) 2
images/2048title.raw: images/2048title.bmp
	$(IMG2RAW) 30
