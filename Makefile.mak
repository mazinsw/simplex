CPP    = g++.exe
WINDRES= windres.exe
OBJS   =lib\window.o \
        lib\radiobutton.o \
        lib\panel.o \
        lib\label.o \
        lib\groupbox.o \
        lib\edit.o \
        lib\dialog.o \
        lib\control.o \
        lib\checkbox.o \
        lib\button.o \
        lib\image.o \
        lib\font.o \
        lib\imageUtil.o \
        lib\menu.o \
        lib\progressbar.o \
        lib\strings.o \
        src\main.o \
        src\number.o \
        src\expviewer.o \
        src\parser.o \
        src\simplexview.o \
        src\matrix.o \
        AppResource.res

LIBS   = -L"$(MINGW_PATH)\lib" -mwindows -limg -lgdi32 -luser32
CFLAGS = -I"$(MINGW_PATH)\include"

all: build

build: $(OBJS)
	$(CPP) -Wall -s -O2 -o Simplex.exe $(OBJS) $(LIBS)

lib\window.o: lib\window.cpp
	$(CPP) -Wall -s -O2 -c lib\window.cpp -o lib\window.o $(CFLAGS)

lib\radiobutton.o: lib\radiobutton.cpp
	$(CPP) -Wall -s -O2 -c lib\radiobutton.cpp -o lib\radiobutton.o $(CFLAGS)

lib\panel.o: lib\panel.cpp
	$(CPP) -Wall -s -O2 -c lib\panel.cpp -o lib\panel.o $(CFLAGS)

lib\label.o: lib\label.cpp
	$(CPP) -Wall -s -O2 -c lib\label.cpp -o lib\label.o $(CFLAGS)

lib\groupbox.o: lib\groupbox.cpp
	$(CPP) -Wall -s -O2 -c lib\groupbox.cpp -o lib\groupbox.o $(CFLAGS)

lib\edit.o: lib\edit.cpp
	$(CPP) -Wall -s -O2 -c lib\edit.cpp -o lib\edit.o $(CFLAGS)

lib\dialog.o: lib\dialog.cpp
	$(CPP) -Wall -s -O2 -c lib\dialog.cpp -o lib\dialog.o $(CFLAGS)

lib\control.o: lib\control.cpp
	$(CPP) -Wall -s -O2 -c lib\control.cpp -o lib\control.o $(CFLAGS)

lib\checkbox.o: lib\checkbox.cpp
	$(CPP) -Wall -s -O2 -c lib\checkbox.cpp -o lib\checkbox.o $(CFLAGS)

lib\button.o: lib\button.cpp
	$(CPP) -Wall -s -O2 -c lib\button.cpp -o lib\button.o $(CFLAGS)

lib\image.o: lib\image.cpp
	$(CPP) -Wall -s -O2 -c lib\image.cpp -o lib\image.o $(CFLAGS)

lib\font.o: lib\font.cpp
	$(CPP) -Wall -s -O2 -c lib\font.cpp -o lib\font.o $(CFLAGS)

lib\imageUtil.o: lib\imageUtil.cpp
	$(CPP) -Wall -s -O2 -c lib\imageUtil.cpp -o lib\imageUtil.o $(CFLAGS)

lib\menu.o: lib\menu.cpp
	$(CPP) -Wall -s -O2 -c lib\menu.cpp -o lib\menu.o $(CFLAGS)

lib\progressbar.o: lib\progressbar.cpp
	$(CPP) -Wall -s -O2 -c lib\progressbar.cpp -o lib\progressbar.o $(CFLAGS)

lib\strings.o: lib\strings.cpp
	$(CPP) -Wall -s -O2 -c lib\strings.cpp -o lib\strings.o $(CFLAGS)

src\main.o: src\main.cpp
	$(CPP) -Wall -s -O2 -c src\main.cpp -o src\main.o $(CFLAGS)

src\number.o: src\number.cpp
	$(CPP) -Wall -s -O2 -c src\number.cpp -o src\number.o $(CFLAGS)

src\expviewer.o: src\expviewer.cpp
	$(CPP) -Wall -s -O2 -c src\expviewer.cpp -o src\expviewer.o $(CFLAGS)

src\parser.o: src\parser.cpp
	$(CPP) -Wall -s -O2 -c src\parser.cpp -o src\parser.o $(CFLAGS)

src\simplexview.o: src\simplexview.cpp
	$(CPP) -Wall -s -O2 -c src\simplexview.cpp -o src\simplexview.o $(CFLAGS)

src\matrix.o: src\matrix.cpp
	$(CPP) -Wall -s -O2 -c src\matrix.cpp -o src\matrix.o $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

