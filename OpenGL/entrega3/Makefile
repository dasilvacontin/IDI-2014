CCFLAGS = -g
MODEL_FLAGS = ../Model/model.cpp GameObject.cc GameModel.cc
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL -lGLU
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: new_scene

new_scene: new_scene.cc
	g++ ${CCFLAGS} -o new_scene.exe new_scene.cc ${MODEL_FLAGS}
	make rm_garbage

rm_garbage:
	rm -rf *.dSYM *~

clean:
	rm -rf new_scene.exe
	make rm_garbage
