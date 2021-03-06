#
# IMS Player Makefile
# inhak.min@gmail.com
#

CXX = g++
TARGET = imsplayer
UNAME= $(shell uname)

ifeq ($(UNAME), MINGW32_NT-5.1)
WXCONFIG = ./lib/wxwidgets/bin/wx-config
else
WXCONFIG = wx-config
endif

SDLCONFIG = sdl-config

CFLAGS = -W -Wno-write-strings -I. `$(SDLCONFIG) --cflags`
CFLAGS += -g

CFLAGS += `$(WXCONFIG) --cflags`
ifeq ($(UNAME), MINGW32_NT-5.1)
	LDFLAGS =  -lwx_mswu_stc-2.8 `$(WXCONFIG) --libs`
	LDFLAGS += -Wl,--enable-auto-import
else
	LDFLAGS =  `$(WXCONFIG) --libs` `$(WXCONFIG) --libs aui`
endif

ifeq ($(UNAME), MINGW32_NT-5.1)
#    LDFLAGS += -static
endif

LDFLAGS += `$(SDLCONFIG) --libs`

.SUFFIXES:
.SUFFIXES: .cpp .o

SRC = fmopl.cpp \
	  outchip.cpp \
	  adlib.cpp \
	  convert.cpp \
	  setfreq.cpp \
	  Bnk.cpp \
	  Ims.cpp \
	  PeakMeter.cpp \
	  ActionPanel.cpp \
	  IssViewer.cpp \
	  SliderPanel.cpp \
	  Sound.cpp \
	  GButton.cpp \
	  GSlider.cpp \
	  Hangul.cpp \
	  Iss.cpp \
	  GHanLabel.cpp \
	  Main.cpp

OBJECTS = $(SRC:.cpp=.o)

ifeq ($(UNAME), MINGW32_NT-5.1)
	OBJECTS += imsplayer.res
endif

.cpp.o : ; $(CXX) -c $(CFLAGS) -o $*.o $<

%.res : %.rc
	windres `$(WXCONFIG) --cppflags -rescomp` -i $< -J rc -o $@ -O coff

all: $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(UNAME), MINGW32_NT-5.1)
	$(CXX) -o $@ -mthreads -Wl,--subsystem,windows -mwindows \
		$(OBJECTS) $(LDFLAGS) 
else
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) 
endif

clean:
ifeq ($(UNAME), MINGW32_NT-5.1)
	rm -f *.o *.res $(TARGET).exe
else
	rm -f *.o $(TARGET)
endif
