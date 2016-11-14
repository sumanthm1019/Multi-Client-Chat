#ifndef __CLIENT__
#define __CLIENT__
#include <string>

inline int send_msg		(int server_socket, char *msg);
int send_file	(int server_socket, char *file_name);

int recv_file	(int server_socket, char *file_name);
inline int recv_msg		(int server_socket, int len);

#endif
