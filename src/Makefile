NAME := Smiril-smstools
MAJOR4 := 0
MINOR4 := 1
MAJOR3 := 0
MINOR3 := 1
MAJOR2 := 0
MINOR2 := 4
MAJOR1 := 0
MINOR1 := 4
CODENAME1 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m
CODENAME2 := T.E.D. - The Enemy Dail
CODENAME3 := \x1B[32mT.E.D.\x1B[39m - \x1B[33mThe Enemy Dail\x1B[39m
LIBNAME := $(NAME)
VERSION1 := $(LIBNAME) $(MAJOR1).$(MINOR1) $(CODENAME1)
VERSION2 := $(LIBNAME) $(MAJOR2).$(MINOR2) $(CODENAME2)
VERSION3 := $(LIBNAME) $(MAJOR3).$(MINOR3) $(CODENAME3)
VERSION4 := $(LIBNAME) $(MAJOR4).$(MINOR4) $(CODENAME3)
LIBS := `pkg-config gtk+-2.0 --cflags --libs`
PREFIX := /bin
CXX := g++
CX1 := i686-w64-mingw32-g++
CX2 := i686-darwin-mingw32-g++
CXXFLAGS1 := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION1)\""
CXXFLAGS2 := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION2)\""
CXXFLAGS3 := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION3)\""
CXXFLAGS4 := -Wall -Wextra -pedantic \
        -Wno-variadic-macros \
        -DVERSION="\"$(VERSION4)\""
        
.PHONY: all smsdeamon smstools3 debug-smstools3 smstools3-gtk debug-smstools3-gtk debug-W32smstools W32smstools x64smstools debug-x64smstools debug-W32smstools-gtk W32smstools-gtk x64smstools-gtk debug-x64smstools-gtk debug-W32getsms x64getsms debug-x64getsms W32getsms getsms debug-getsms install uninstall clean version

all: smstools3 smstools3-gtk getsms smsdeamon 
smsdeamon: main-d.cc	
	$(CXX) main-d.cc $(CXXFLAGS4) -o smsdeamon -lpthread
debug-smsdeamon: CXXFLAGS4 += -g -DDEBUG
debug-smsdeamon: main-d.cc
	$(CXX) $(CXXFLAGS4) main-d.cc -o debug-smsdeamon -lpthread
smstools3: main-c.cc	
	$(CXX) main-c.cc $(CXXFLAGS1) -o smstools 
debug-smstools3: CXXFLAGS1 += -g -DDEBUG
debug-smstools3: main-c.cc
	$(CXX) $(CXXFLAGS1) main-c.cc -o debug-smstools 
W32smstools: main-c.cc	
	$(CX1) main-c.cc $(CXXFLAGS1) -m32 -o W32smstools.exe
debug-W32smstools: CXXFLAGS1 += -g -DDEBUG
debug-W32smstools: main-c.cc
	$(CX1) $(CXXFLAGS1) main-c.cc -m32 -o debug-W32smstools.exe 
x64smstools: main-c.cc	
	$(CX1) main-c.cc $(CXXFLAGS1) -m64 -o x64smstools.exe
debug-x64smstools: CXXFLAGS1 += -g -DDEBUG
debug-x64smstools: main-c.cc
	$(CX1) $(CXXFLAGS1) main-c.cc -m64 -o debug-x64smstools.exe
smstools3-gtk: main-gtk.cc	
	$(CXX) main-gtk.cc $(CXXFLAGS2) -o smstools-gtk $(LIBS)
debug-smstools3-gtk: CXXFLAGS2 += -g -DDEBUG
debug-smstools3-gtk: main-gtk.cc
	$(CXX) $(CXXFLAGS2) main-gtk.cc -o debug-smstools-gtk $(LIBS) 
W32smstools-gtk: main-gtk.cc
	$(CX1) $(CXXFLAGS2) main-gtk.cc -m32 -o W32smstools-gtk.exe $(LIBS)
debug-W32smstools-gtk: CXXFLAGS1 += -g -DDEBUG
debug-W32smstools-gtk: main-gtk.cc
	$(CX1) $(CXXFLAGS1) main-gtk.cc -m32 -o debug-W32smstools-gtk.exe $(LIBS)
x64smstools-gtk: main-gtk.cc
	$(CX1) $(CXXFLAGS2) main-gtk.cc -m64 -o x64smstools-gtk.exe $(LIBS)
debug-x64smstools-gtk: CXXFLAGS1 += -g -DDEBUG
debug-x64smstools-gtk: main-gtk.cc
	$(CX1) $(CXXFLAGS1) main-gtk.cc -m64 -o debug-x64smstools-gtk.exe $(LIBS)
getsms: main.cc
	$(CXX) main.cc $(CXXFLAGS3) -o getsms
debug-getsms: CXXFLAGS3 += -g -DDEBUG
debug-getsms: main.cc
	$(CXX) $(CXXFLAGS3) main.cc -o debug-getsms 
W32getsms: main.cc
	$(CX1) main.cc $(CXXFLAGS3) -m32 -o W32getsms.exe
debug-W32getsms: CXXFLAGS3 += -g -DDEBUG
debug-W32getsms: main.cc
	$(CX1) $(CXXFLAGS3) main.cc -m32 -o debug-W32getsms.exe
x64getsms: main.cc
	$(CX1) main.cc $(CXXFLAGS3) -m64 -o x64getsms.exe
debug-x64getsms: CXXFLAGS3 += -g -DDEBUG
debug-x64getsms: main.cc
	$(CX1) $(CXXFLAGS3) main.cc -m64 -o debug-x64getsms.exe
version:
	@echo $(VERSION4)
	@echo $(VERSION3)
	@echo $(VERSION2)
	@echo $(VERSION1)

install: getsms smstools3 smstools3-gtk
	mkdir -p $(PREFIX)
	cp smstools $(PREFIX)
	chmod +x $(PREFIX)/smstools
	cp smstools-gtk $(PREFIX)
	chmod +x $(PREFIX)/smstools-gtk
	cp getsms $(PREFIX)
	chmod +x $(PREFIX)/getsms
	cp smsdeamon $(PREFIX)
	chmod +x $(PREFIX)/smsdeamon
	
uninstall: 
	rm -rf $(PREFIX)/smstools-gtk
	rm -rf $(PREFIX)/smstools
	rm -rf $(PREFIX)/getsms
	rm -rf $(PREFIX)/smsdeamon
clean: 
	rm -rf smsdeamon
	rm -rf smstools-gtk
	rm -rf smstools
	rm -rf getsms
	rm -rf debug-smstools	
	rm -rf debug-smstools-gtk
	rm -rf debug-getsms
	rm -rf W32smstools-gtk.exe
	rm -rf W32smstools.exe
	rm -rf W32getsms.exe
	rm -rf debug-W32smstools.exe	
	rm -rf debug-W32smstools-gtk.exe
	rm -rf debug-W32getsms.exe
	rm -rf x64smstools-gtk.exe
	rm -rf x64smstools.exe
	rm -rf x64getsms.exe
	rm -rf debug-x64smstools.exe	
	rm -rf debug-x64smstools-gtk.exe
	rm -rf debug-x64getsms.exe
