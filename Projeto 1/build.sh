gcc -w -o3 -g -c src/client_tcp.c -o build/client_lib.o
gcc -w -o3 -g -c src/client.c -o build/client.o

gcc -o build/client build/client.o build/client_lib.o

gcc -w -g -o3 -o build/server $(mysql_config --cflags)  src/server_tcp.c src/server.c $(mysql_config --libs)