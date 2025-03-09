RAYLIB ?= ./external/raylib-5.5/src/

qtree:
	gcc src/quadtree.c

game:
	gcc src/main.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
