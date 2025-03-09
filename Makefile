RAYLIB ?= ./external/raylib-5.5/src/

all:
	gcc src/main.c src/quadtree.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
