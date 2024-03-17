CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SERVER_EXEC = server
CLIENT_EXEC = client

.PHONY: all clean

all: $(SERVER_EXEC)

$(SERVER_EXEC): server.c
	$(CC) $(CFLAGS) -o $@ $<

run_server: $(SERVER_EXEC)
	./$(SERVER_EXEC)

$(CLIENT_EXEC): client.c
	$(CC) $(CFLAGS) -o $@ $<

run_client: $(CLIENT_EXEC)
	./$(CLIENT_EXEC)


clean:
	rm -f $(SERVER_EXEC) ${CLIENT_EXEC}
