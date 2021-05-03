#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4100

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);

					 switch (strtok(buffer, " ")[0]) {
						case '1':
							strcpy(buffer, "createNewProfile");
							break;

						case '2':
							strcpy(buffer, "addExperience");
							break;

						case '3':
							strcpy(buffer, "peopleWithCourse");
							break;

						case '4':
							strcpy(buffer, "peopleWitHabiliity");
							break;

						case '5':
							strcpy(buffer, "peopleWithConclusionYear");
							break;

						case '6':
							strcpy(buffer, "listAll");
							break;

						case '7':
							strcpy(buffer, "personInformation");
							break;

						case '8':
							strcpy(buffer, "removePerson");
							break;

						default:
							strcpy(buffer, "1 - cadastrar um novo perfil utilizando o email como identificador\n2 - acrescentar uma nova experiência profissional em um perfil\n3 - listar todas as pessoas (email e nome) formadas em um determinado curso\n4 - listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n5 - listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n6 - listar todas as informações de todos os perfis\n7 - dado o email de um perfil, retornar suas informações\n8 - remover um perfil a partir de seu identificador (email)\n");
							break;
					}

					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}