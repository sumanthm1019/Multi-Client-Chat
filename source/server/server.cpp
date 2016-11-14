#include "utilities.h"
#include "server.h"
#include "client.h"

#define MAX_CLIENTS	 (10)


static map *root_map;

static int recv_packet	(int client_socket, pkt_t *packet);

static void insert_client(map** root, int socket, char* name);

static int lookup_client(map* root, char* name);

//Insert a new client into the LinkedList data structure
static void insert_client(map** root_map, int socket, char* name){

	map* new_node = (map *) malloc(sizeof(map));
	strcpy (new_node->name, name);
	new_node->socket_id=socket;

	if(*root_map==NULL){
		//First CLient
		*root_map= new_node;
		new_node->next=NULL;
	}else{
		//Else add new client and make it as root
		new_node->next=*root_map;
		*root_map= new_node;
	}


}


//Given client name, lookup client_socket in the map  linklist and return
static int lookup_client(map* root, char* name){
	if (root ==NULL){
		printf("No client is connected server");
		return -1;
	}

	while(root!=NULL){
		if (strcmp(root->name,name)==0){
			return root->socket_id;
		}else
			root=root->next;
	}

	printf("Client with Name:%s not connected to server", name);
	return -1;

}


static int recv_packet(int client_socket, pkt_t *packet) {

	int recv_status = recv(client_socket, packet, sizeof(pkt_t), 0);
	if (recv_status == -1) {
		ERROR("Receiving first packet!");
		return 1;
	}
	if (recv_status == 0){
		ERROR("Client Disconnected");
		return 1;
	}
	if (packet == NULL)
		return 1;

	int len = packet->len;
	char *msg    = (char *) malloc(len);
	packet->data = (char *) malloc(len);

	recv_status = recv(client_socket, msg, len, 0);
	if (recv_status == -1) {
		ERROR("Receiving main packet!");
		return 1;
	}

	memcpy(packet->data, msg, len);
	free(msg);
	printf("message is %s\n", packet->data);

	return 0;
}





int main(int argc, char *argv[]) {

	printf("Server\n");


	char client_name[MAX_NAME_LEN] = {0};	//Hold Client Name for mapping

	int server_socket, client_socket;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket==-1){
		ERROR("Creating server_socket\n");
		return 1;
    }


	// init structure for server socket
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket with the struct

	if (bind(server_socket, (struct sockaddr *) &server_address,
			sizeof(server_address)) == -1){
			ERROR("Binding server_socket\n");
			return 1;
	}

	// listening for connections
	if (listen(server_socket, MAX_CLIENTS) == -1){
		ERROR("Listening to server_socket");
		return 1;
	}

	root_map = NULL;

	while(1){
		//Initialization step
		//Accept new client
		//Receive Client name and map it to the client-fd
		client_socket = accept(server_socket, NULL, NULL);
		recv(client_socket, client_name, sizeof(client_name), 0);
		insert_client(&root_map,client_socket,client_name);
		// TODO: Create thread 
	}

	return 0;
}
