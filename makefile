all:
	gcc exponencial.c -lm  -o exponencial

run:
	gcc exponencial.c -lm  -o exponencial
	./exponencial

clean:
	rm exponencial