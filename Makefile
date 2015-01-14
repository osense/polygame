TARGET := Polygame

SOURCES := $(wildcard jni/*.cpp) $(wildcard jni/*/*.cpp)
OBJS := $(patsubst jni/%.cpp,obj/%.o,$(SOURCES))

IRRLICHT := ../irrlicht-ogl-es


USERCPPFLAGS := -I../irrpp/include -DDEBUG_GLES -g -Wall -ffast-math -std=c++11

USERLDFLAGS := -L../irrpp/lib/linux/debug -lirrPP


CPPFLAGS := -MMD -I$(IRRLICHT)/include -I/usr/X11R6/include $(USERCPPFLAGS) -Ijni
LDFLAGS := $(USERLDFLAGS) -L$(IRRLICHT)/lib/Linux -lIrrlicht -lXxf86vm -lX11 -lEGL -lGL -lGLESv1_CM


all: $(OBJS)
	g++ $(CPPFLAGS) $^ -o $(TARGET) $(LDFLAGS)

obj/%.o: jni/%.cpp
	mkdir -p $$(dirname $@)
	g++ $(CPPFLAGS) -c -o $@ $<

-include $(OBJS:.o=.d)

clean:
	rm -rf obj


.PHONY: all clean
