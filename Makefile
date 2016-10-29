
INC = -I./inc/

all:

		g++ -Wall $(INC) -g -o ./bin/server ./source/server/server.cpp -pthread
		g++ -Wall $(INC) -g -o ./bin/client ./source/client/client.cpp -pthread
				
clean:
		rm -rf *o *so *out
		rm -rf /run/shm/*txt