CCFLAGS = -g
MODEL_FLAGS = ../Model/model.cpp GameObject.cc GameModel.cc
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: teacupViewer snowman homerViewer legomanViewer entrega2

teacupViewer: teacupViewer.cc
	g++ ${CCFLAGS} -o teacupViewer teacupViewer.cc

snowman: snowman.cc
	g++ ${CCFLAGS} -o snowman snowman.cc

homerViewer: homerViewer.cc
	g++ ${CCFLAGS} -o homerViewer homerViewer.cc ${MODEL_FLAGS}

legomanViewer: legomanViewer.cc
	g++ ${CCFLAGS} -o legomanViewer legomanViewer.cc ${MODEL_FLAGS}

entrega2: entrega2.cc
	g++ ${CCFLAGS} -o entrega2 entrega2.cc ${MODEL_FLAGS}

clean:
	rm -rf *.dSYM *~ homerViewer legomanViewer entrega2
