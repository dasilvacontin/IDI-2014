CCFLAGS = -g
MODEL_FLAGS = ../Model/model.cpp GameObject.cc GameModel.cc
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: e3_orthogonal

e3_orthogonal: e3_orthogonal.cc
	g++ ${CCFLAGS} -o e3_orthogonal.exe e3_orthogonal.cc ${MODEL_FLAGS}

clean:
	rm -rf *.dSYM *~ e3_orthogonal.exe