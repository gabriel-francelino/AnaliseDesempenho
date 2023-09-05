all:
	gcc exponencial.c -lm  -o exponencial

run:
	./exponencial

clean:
	rm exponencial