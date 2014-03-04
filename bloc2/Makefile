CCFLAGS = -g
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: snowman

main: main.cc
	g++ ${CCFLAGS} -o main main.cc

teacupViewer: teacupViewer.cc
	g++ ${CCFLAGS} -o teacupViewer teacupViewer.cc

snowman: snowman.cc
	g++ ${CCFLAGS} -o snowman snowman.cc

clean:
	rm main