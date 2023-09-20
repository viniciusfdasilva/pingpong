CC=gcc

all:
	@$(CC) client.c -o client
	@$(CC) server.c -o server
	@./server $(buffer_size)&
	@./client $(buffer_size)

clean:
	@rm server client

run_client:
	@$(CC) client.c -o client
	@./client $(buffer_size)
	@rm server client

run_server:
	@$(CC) server.c -o server
	@./server $(buffer_size)&

graphic:
	@python graphic.py