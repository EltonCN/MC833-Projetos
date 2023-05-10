gcc -o build/client -w -O3 src/client.c src/client_tcp.c 
gcc -o build/server $(mysql_config --cflags) src/server_tcp.c src/server.c $(mysql_config --libs) -D _GNU_SOURCE

gcc -o build/serverUDP $(mysql_config --cflags) src/server_udp.c src/server.c $(mysql_config --libs) -D _GNU_SOURCE