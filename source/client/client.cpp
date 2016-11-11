#include "utilities.h"
#include "client.h"

#define NUM_CLIENT_THREADS 3

static pkt_t client_packet;

static int build_packet(char *cast_type, char *pkt_type, char *data);
static int send_packet(int server_socket, pkt_t *packet);

static int send_packet(int server_socket, pkt_t *packet) {
	pkt_t *first_packet = (pkt_t *) malloc(sizeof(pkt_t));

	first_packet->cast_type = packet->cast_type;
	first_packet->len = packet->len;
	first_packet->pkt_type = packet->pkt_type;
	first_packet->data = NULL;

	int send_status = send(server_socket, first_packet, sizeof(pkt_t), 0);
	if (send_status == -1) {
		ERROR("Sending first packet!\n");
		return 1;
	}

	printf("data sending %s %d \n", packet->data, packet->len);
	send_status = send(server_socket, packet->data, packet->len + 1, 0);
	if (send_status == -1) {
		ERROR("Sending main packet!\n");
		return 1;
	}

	return 0;
}

static int build_packet(char *cast_type, char *pkt_type, char *data) {

	if (cast_type == NULL || pkt_type == NULL || data == NULL) {
		ERROR("Enter correct arguments!\n");
		return 1;
	}

	if (!strcmp(cast_type, "unicast")) {
		client_packet.cast_type = UNICAST;
	} else if (!strcmp(cast_type, "broadcast")) {
		client_packet.cast_type = BROADCAST;
	} else {
		client_packet.cast_type = BLOCKCAST;
	}

	if (!strcmp(pkt_type, "message")) {
		client_packet.pkt_type = MESSAGE;
		client_packet.len = strlen(data);
		client_packet.data = data;
	} else {
		client_packet.pkt_type = FILE;
	}

	return 0;
}

void *user_interface(void *args) {
	char pkt_type[10];
	char cast_type[10];
	char text_link[100];
	while (1) {
		printf("Client:");
		scanf("%s %s %s", cast_type, pkt_type, text_link);
		if (cast_type == NULL || pkt_type == NULL || text_link == NULL) {
			ERROR("Entered parameters incorrect\n");
			printf(
					"Usage: unicast/broadcast/blockcast message/file message-content/file-path");
			continue;
		}
		//pthread_mutex_lock();
		int build_packet_status = build_packet(cast_type, pkt_type, text_link);
		if (build_packet_status != 0) {
			ERROR("building packet!\n");
		}
		//pthread_mutex_unlock();
		//pthread_cond_signal();
	}

	return NULL;
}

void *comm_interface(void *args) {
	int server_socket = *(int *) args;
	while (1) {
		//pthread_mutex_lock();
		//pthread_cond_wait();
		int send_status = send_packet(server_socket, &client_packet);
		if (send_status != 0) {
			ERROR("Sending message to server!\n");
		}
		//pthread_mutex_unlock();

	}

	return NULL;
}

int main(int argc, char *argv[]) {

	int server_socket;

	if (argc != 2) {
		printf("Usage: %s <name_of_client>\n", argv[0]);
		return 0;
	}
	pthread_t tid[NUM_CLIENT_THREADS];

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		ERROR("Socket creation!\n");
		return 1;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(server_socket,
			(struct sockaddr *) &server_address, sizeof(server_address));
	if (connection_status == -1) {
		ERROR("Error making connection at server end\n");
		return 1;
	}

	int send_status = send(server_socket, argv[1], strlen(argv[1]), 0);
	if (send_status == -1) {
		ERROR("Sending first packet!\n");
		return 1;
	}

	pthread_create(&tid[0], NULL, user_interface, NULL);
	//pthread_create(&tid[1], NULL, comm_interface, &server_socket);

	pthread_join(tid[0], NULL);
	//pthread_join(tid[1], NULL);
	close(server_socket);
	return 0;
}

