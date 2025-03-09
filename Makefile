RAYLIB ?= ./external/raylib-5.5/src/

points:
	gcc src/points/main.c src/points/quadtree.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

comp:
	gcc src/comp/main.c src/comp/quadtree.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
