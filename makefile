all:
	gcc simulacao.c -lm  -o simulacao

run:
	gcc simulacao.c -lm  -o simulacao
	./simulacao

clean:
	rm simulacao