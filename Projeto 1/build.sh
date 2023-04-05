gcc -w -o3 -c src/client_tcp.c -o build/client_lib.o
gcc -w -o3 -c src/client.c -o build/client.o

gcc -o build/client build/client.o build/client_lib.o