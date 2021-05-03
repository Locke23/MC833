#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8338

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(*str)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(*str)*len);
}

void handleCreateNewProfile (int clientSocket) {
	char email[20];
	printf("Digite o email\n");
	scanf("%s", email);

	char nome[20];
	printf("Digite o nome\n");
	scanf("%s", nome);

	char* sobrenome;
	int temp;
	printf("Digite o sobrenome\n");
  scanf("%d",&temp); // temp statement to clear buffer
	sobrenome = inputString(stdin, 20);

	char* residencia;
	printf("Digite a cidade\n");
  scanf("%d",&temp); // temp statement to clear buffer
	residencia = inputString(stdin, 20);

	char* formacaoAcad;
	printf("Digite o formação acadêmica\n");
	formacaoAcad = inputString(stdin, 20);

	char* anoFormatura;
	printf("Digite o ano de formatura\n");
	anoFormatura = inputString(stdin, 20);
	
	char* habilidades;
	printf("Digite suas habilidades no seguinte formato:\n");
	printf("Análise de Dados, Internet das Coisas, Computação em Nuvem\n");
  scanf("%d",&temp); // temp statement to clear buffer
	habilidades = inputString(stdin, 20);

	char* buffer = "1";
	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,nome);

	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,sobrenome);

	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,residencia);

	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,formacaoAcad);
	
	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,anoFormatura);

	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,habilidades);

	send(clientSocket, buffer, strlen(buffer), 0);
}

void handleAddExperience (int clientSocket) {
	char email[20];
	printf("Digite o email\n");
	scanf("%s", email);

	char* experience;
	int temp;
	printf("Digite a experiencia\n");
  scanf("%d",&temp); // temp statement to clear buffer
	experience = inputString(stdin, 20);
	
	char* buffer = "2";
	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,email);

	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s\n",buffer,experience);

	send(clientSocket, buffer, strlen(buffer), 0);

}

void handlePeopleWithCourse (int clientSocket) {
	char* formacaoAcad;
	int temp;
	printf("Digite o formação acadêmica para a busca\n");
  scanf("%d",&temp);
	formacaoAcad = inputString(stdin, 20);

	char* buffer = "3";
	asprintf(&buffer,"%s&",buffer);
	asprintf(&buffer,"%s%s",buffer,formacaoAcad);

	send(clientSocket, buffer, strlen(buffer), 0);
	
}

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		// send(clientSocket, buffer, strlen(buffer), 0);
		
		switch (strtok(buffer, " ")[0]) {
			case '1':
				handleCreateNewProfile(clientSocket);
				break;

			case '2':
				handleAddExperience(clientSocket);
				break;

			case '3':
				handlePeopleWithCourse(clientSocket);
				break;

			default:
				break;
		}
		
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \n%s\n", buffer);
		}
	}

	return 0;
}



// documentation

// createNewProfile
// 1&email&nome&...

// addExperience
// 2&email&Experience

// peopleWithCourse
// 3 + course

// peopleWitHabiliity
// 4 + hability

// peopleWithConclusionYear
// 5 + conclusionYear

// listAll
// 6

// personInformation
// 7 + email

// removePerson
// 8 + email

// help
// other texts
