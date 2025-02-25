all: tarea_0

tarea_0: tarea_0.c
	gcc -o tarea_0 tarea_0.c `pkg-config --cflags --libs gtk+-3.0 cairo` -lm

clean:
	rm -f ./tarea_0 || true

