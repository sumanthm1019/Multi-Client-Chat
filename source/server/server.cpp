#include "utilities.h"
#include "server.h"

#define MAX_CLIENTS	(10)

static clientSocketMap_t *map;


static int recv_packet(int client_socket, pkt_t *packet);


static int recv_packet(int client_socket, pkt_t *packet) {


	int recv_status = recv(client_socket, packet, sizeof(pkt_t), 0);
	if (recv_status == -1) {
		ERROR("Receiving first packet!\n");
		return 1;
	}

	int len = packet->len + 1;
	char *msg = (char *) malloc(len);
	packet->data = (char *) malloc(len);

	recv_status = recv(client_socket, msg, len, 0);
	if (recv_status == -1) {
		ERROR("Receiving main packet!\n");
		return 1;
	}

	memcpy(packet->data, msg, len);
	free(msg);
	printf("message is %s\n", packet->data);

	return 0;
}
int main(int argc, char *argv[]) {

	printf("Server\n");

	int server_socket, client_socket;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);

	// init structure for server socket
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket with the struct

	bind(server_socket, (struct sockaddr*) &server_address,
			sizeof(server_address));
	// listening for connections
	listen(server_socket, MAX_CLIENTS);
	static pkt_t packet;
	client_socket = accept(server_socket, NULL, NULL);
	map = (clientSocketMap_t *) malloc(sizeof(clientSocketMap_t));
	while (1) {



		int recv_status = recv_packet(client_socket, &packet);
		if (recv_status == -1) {
			ERROR("Receiving packet!\n");
			return 1;
		}

	}
	return 0;
}
