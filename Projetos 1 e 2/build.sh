gcc -o build/client_tcp -w -O3 src/common_def.c src/client.c src/client_tcp.c 
gcc -o build/client_udp -w -O3  src/common_def.c src/client.c src/client_udp.c 
gcc -o build/server $(mysql_config --cflags) src/server_tcp.c src/server.c $(mysql_config --libs) -D _GNU_SOURCE
gcc -o build/serverUDP $(mysql_config --cflags) src/server_udp.c src/server.c $(mysql_config --libs) -D _GNU_SOURCE
