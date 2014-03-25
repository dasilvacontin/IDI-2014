CCFLAGS = -g
MODEL_FLAGS = ../Model/model.cpp GameObject.cc GameModel.cc
UNAME = $(shell uname -s)

ifeq ($(UNAME),Linux)
    CCFLAGS += -lglut -lGL -lGLU
endif
ifeq ($(UNAME),Darwin)
    CCFLAGS += -framework OpenGL -framework GLUT -Wno-deprecated
endif

all: e3_orthogonal e3_euler rm_garbage exercici1 new_scene

e3_orthogonal: e3_orthogonal.cc
	g++ ${CCFLAGS} -o e3_orthogonal.exe e3_orthogonal.cc ${MODEL_FLAGS}
	make rm_garbage

e3_euler: e3_euler.cc
	g++ ${CCFLAGS} -o e3_euler.exe e3_euler.cc ${MODEL_FLAGS}
	make rm_garbage

e3_pers: e3_pers.cc
	g++ ${CCFLAGS} -o e3_pers.exe e3_pers.cc ${MODEL_FLAGS}
	make rm_garbage

exercici1: exercici1.cc
	g++ ${CCFLAGS} -o exercici1.exe exercici1.cc ${MODEL_FLAGS}
	make rm_garbage

new_scene: new_scene.cc
	g++ ${CCFLAGS} -o new_scene.exe new_scene.cc ${MODEL_FLAGS}
	make rm_garbage

rm_garbage:
	rm -rf *.dSYM *~

clean:
	rm -rf e3_orthogonal.exe e3_euler.exe e3_pers.exe exercici1.exe new_scene.exe
	make rm_garbage
