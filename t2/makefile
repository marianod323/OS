all:
	gcc -c curl_code.c -lcurl
	gcc -o Sete_Cliques_serial curl_code.o Sete_Cliques_serial.c -lcurl
	gcc -o Sete_Cliques_threads curl_code.o Sete_Cliques_threads.c -lcurl -lpthread
clean:
	rm curl_code.o
	rm Sete_Cliques_serial
	rm Sete_Cliques_threads
