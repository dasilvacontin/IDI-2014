CCFLAGS = -g
MODEL_FLAGS = ../Model/model.cpp GameObject.cc GameModel.cc
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL -lGLU
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: lights

lights: lights.cc
	g++ ${CCFLAGS} -o lights.exe lights.cc ${MODEL_FLAGS}
	make rm_garbage

rm_garbage:
	rm -rf *.dSYM *~

clean:
	rm -rf 	lights.exe
	make rm_garbage
