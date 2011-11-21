#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
CFLAGS = -O3 -fsingle-precision-constant -fPIC
GENERAL_TWEAKS = -DFAST_BUT_UGLY -DFAST_BUT_UGLY_2 -ffast-math -fgcse-lm -fgcse-sm -fsched-spec-load
#==PC==
CPP = gcc -g -march=native -DX86CPU
SDL = `sdl-config --cflags`
#==Consoles==
#==GP2X/WIZ==
#  REALGP2X activates gp2xspecific optimizations, which the wiz doesn't need
#	 e.g. modells with fewer polygons
#ORIGINALFW = -static -lsmpeg -lstdc++ -lSDL -lfreetype -lpng -lpthread -lz -ljpeg -lm -s
ifeq ($(TARGET),gp2x)
CPP = /opt/open2x/gcc-4.1.1-glibc-2.3.6/bin/arm-open2x-linux-gcc -DMOBILE_DEVICE -DARMCPU -DGP2X $(GENERAL_TWEAKS) -DFAST_BUT_UGLY_2 -DREALGP2X
SDL = `/opt/open2x/gcc-4.1.1-glibc-2.3.6/bin/sdl-config --cflags`
INCLUDE = -I/opt/open2x/gcc-4.1.1-glibc-2.3.6/include
LIB = -L/opt/open2x/gcc-4.1.1-glibc-2.3.6/lib -Wl,-rpath=/opt/open2x/gcc-4.1.1-glibc-2.3.6/lib
endif
#==Caanoo==
ifeq ($(TARGET),caanoo)
CPP = /opt/caanoo/gcc-4.2.4-glibc-2.7-eabi/bin/arm-gph-linux-gnueabi-gcc -DMOBILE_DEVICE -DARMCPU -DCAANOO $(GENERAL_TWEAKS)
SDL = -I/opt/caanoo/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
INCLUDE = -I/opt/caanoo/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/include
LIB = -L/opt/caanoo/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/lib -Wl,-rpath=/opt/caanoo/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/lib
endif
#==Dingoo==
ifeq ($(TARGET),dingoo)
CPP = /opt/mipsel-linux-uclibc/usr/bin/mipsel-linux-gcc -DDINGOO $(GENERAL_TWEAKS)
SDL = -I/opt/mipsel-linux-uclibc/usr/mipsel-linux-uclibc/sys-include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
INCLUDE = -I/opt/mipsel-linux-uclibc/usr/mipsel-linux-uclibc/sys-include
LIB = -L/opt/mipsel-linux-uclibc/usr/lib -Wl,-rpath=/opt/mipsel-linux-uclibc/usr/lib
endif
#==Pandora==
ifeq ($(TARGET),pandora)
CPP = /opt/pandora/arm-2011.03/bin/arm-none-linux-gnueabi-gcc -DARMCPU -DPANDORA $(GENERAL_TWEAKS)
SDL = `/opt/pandora/arm-2011.03/usr/bin/sdl-config --cflags`
INCLUDE = -I/opt/pandora/arm-2011.03/usr/include
LIB = -L/opt/pandora/arm-2011.03/usr/lib -Wl,-rpath=/opt/pandora/arm-2011.03/usr/lib -lpnd
endif

all: puzzletube

targets:
	@echo "gp2x (=wiz), caanoo, dingoo, pandora"

puzzletube: puzzletube.c particle.o game.o music.o menu.o stars.o settings.o lettering.o splashscreen.o
	$(CPP) $(CFLAGS) puzzletube.c particle.o game.o music.o menu.o stars.o settings.o lettering.o splashscreen.o $(SDL) $(INCLUDE) $(LIB) -L. -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL -lm  -lsparrow3d $(ORIGINALFW) -o puzzletube

particle.o: particle.c particle.h
	$(CPP) $(CFLAGS) -c particle.c $(SDL) $(INCLUDE)

game.o: game.c game.h
	$(CPP) $(CFLAGS) -c game.c $(SDL) $(INCLUDE)

music.o: music.c music.h
	$(CPP) $(CFLAGS) -c music.c $(SDL) $(INCLUDE)

menu.o: menu.c menu.h
	$(CPP) $(CFLAGS) -c menu.c $(SDL) $(INCLUDE)

stars.o: stars.c stars.h
	$(CPP) $(CFLAGS) -c stars.c $(SDL) $(INCLUDE)

settings.o: settings.c settings.h
	$(CPP) $(CFLAGS) -c settings.c $(SDL) $(INCLUDE)

lettering.o: lettering.c lettering.h
	$(CPP) $(CFLAGS) -c lettering.c $(SDL) $(INCLUDE)

splashscreen.o: splashscreen.c splashscreen.h
	$(CPP) $(CFLAGS) -c splashscreen.c $(SDL) $(INCLUDE)

clean:
	rm *.o
	rm puzzletube
