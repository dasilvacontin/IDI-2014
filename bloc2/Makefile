CCFLAGS = -g
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: main

main: main.cc
	g++ ${CCFLAGS} -o main main.cc

clean:
	rm main