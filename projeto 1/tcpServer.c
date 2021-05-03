#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8306
#define MAXCHAR 1000
void strreplace(char *string, const char *find, const char *replaceWith){
    if(strstr(string, replaceWith) != NULL){
        char *temporaryString = malloc(strlen(strstr(string, find) + strlen(find)) + 1);
        strcpy(temporaryString, strstr(string, find) + strlen(find));    //Create a string with what's after the replaced part
        *strstr(string, find) = '\0';    //Take away the part to replace and the part after it in the initial string
        strcat(string, replaceWith);    //Concat the first part of the string with the part to replace with
        strcat(string, temporaryString);    //Concat the first part of the string with the part after the replaced part
        free(temporaryString);    //Free the memory to avoid memory leaks
    }
}

void personInformation(char* buffer, int newSocket) {
	FILE *fp;
    char str[MAXCHAR];
    char filename[1000] = "data/";
    char profileGetted[2000] = "";
	buffer += 1;
	strcat(filename, buffer);
	strcat(filename, ".txt");
    fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Could not open file %s",filename);
    }
    while (fgets(str, MAXCHAR, fp) != NULL)
        strcat(profileGetted, str);
    fclose(fp);

	strcpy(buffer, profileGetted);
	send(newSocket, buffer, strlen(buffer), 0);
}

// strcpy(filename2, "data/");
// strreplace(str, "\n", "");
// strcat(filename2, str);
// strcat(filename2, ".txt");
// strcpy(profileGetted2, "");
// printf("---- %s\n", filename2);

// fp2 = fopen(filename2, "r");
// if (fp2 == NULL){
// 	printf("Could not open file %s",filename2);
// }
// while (fgets(str, MAXCHAR, fp2) != NULL)
// 	strcat(profileGetted2, str);
// fclose(fp2);
// printf("%s",oneProfile());

// const char* oneProfile() {
// 	FILE *fp2;
// 	char str2[MAXCHAR];
// 	static char profileGetted2[200] = "";

// 	fp2 = fopen("data/gigantelli@unicamp.br.txt", "r");
// 	if (fp2 == NULL){
// 		printf("Could not open file");
// 	}
// 	while (fgets(str2, MAXCHAR, fp2) != NULL)
// 		printf("- %s",str2);
// 		strcat(profileGetted2, str2);
// 	fclose(fp2);

// 	return profileGetted2;
// }

void listAll(char* buffer, int newSocket) {
	FILE *fp;
    char str[MAXCHAR];
    char filename[1000] = "data/index.txt";
    char emails[2000] = "";
	char profilesGetted[2000] = "123";

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
    }
    while (fgets(str, MAXCHAR, fp) != NULL) {
        strcat(emails, str);
	}
    fclose(fp);

	char *ptr = strtok(emails, "\n");
	while (ptr != NULL)
	{
		printf("%s\n", ptr);
		
		char profileURL[1000] = "data/";
		strcat(profileURL, ptr);
		strcat(profileURL, ".txt");
		// printf("%s\n", profileURL);
		fp = fopen(profileURL, "r");
		if (fp == NULL){
			printf("Could not open file");
		}
		while (fgets(str, MAXCHAR, fp) != NULL)
			printf("- %s",str);
		fclose(fp);

		ptr = strtok(NULL, "\n");
	}

	strcpy(buffer, emails);
	send(newSocket, buffer, strlen(buffer), 0);
}

void helpMessage(char* buffer, int newSocket) {
	char helpMessage[2000] = "";
	strcat(helpMessage,"1 - cadastrar um novo perfil utilizando o email como identificador\n");
	strcat(helpMessage, "2 - acrescentar uma nova experiência profissional em um perfil\n");
	strcat(helpMessage, "3 - listar todas as pessoas (email e nome) formadas em um determinado curso\n");
	strcat(helpMessage, "4 - listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n");
	strcat(helpMessage, "5 - listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n");
	strcat(helpMessage, "6 - listar todas as informações de todos os perfis\n");
	strcat(helpMessage, "7 - dado o email de um perfil, retornar suas informações\n");
	strcat(helpMessage, "8 - remover um perfil a partir de seu identificador (email)\n");
	strcpy(buffer, helpMessage);
	send(newSocket, buffer, strlen(buffer), 0);
}



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
					// printUsers();
					 switch (strtok(buffer, " ")[0]) {
						case '1':
							strcpy(buffer, "createNewProfile");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '2':
							strcpy(buffer, "addExperience");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '3':
							strcpy(buffer, "peopleWithCourse");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '4':
							strcpy(buffer, "peopleWitHabiliity");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '5':
							strcpy(buffer, "peopleWithConclusionYear");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '6':
							// strcpy(buffer, "listAll");
							listAll(buffer, newSocket);
							// send(newSocket, buffer, strlen(buffer), 0);
							break;

						case '7':
							personInformation(buffer, newSocket);
							break;

						case '8':
							strcpy(buffer, "removePerson");
							send(newSocket, buffer, strlen(buffer), 0);
							break;

						default:
							helpMessage(buffer, newSocket);
							break;
					}
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}