// $Id$
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#define PORT 13080

int sock;

void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(sock, 2);
  close(sock);
  exit(0);
}



int main(void) {
  printf("Sphynx en attente\n");
  char enonce[64] ="Qui suis je?";
  char reponse[64]  = "Moi";


  /* Rattraper le Ctrl-C */
  signal(SIGINT, close_exit);
  struct sockaddr_in sin;
  socklen_t recsize = sizeof(sin);
  int sock;
  /* Création de la socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);


  /* Socket et contexte d'adressage du client */
  struct sockaddr_in csin;
  int clientSocket;
  socklen_t crecsize = sizeof(csin);


  /* Configuration de la connexion */
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);

  bind(sock, (struct sockaddr*)&sin, crecsize);

  /* Démarrage du listage */
  listen(sock, 1);
while(1){
  clientSocket = accept(sock,(struct sockaddr*)&csin, &crecsize);
  printf("Le serveur de JEU est connecté pour une enigme avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));


    if(send(clientSocket,enonce,sizeof(enonce),0)<0){
      printf("Problem send E\n");
    }else{
      printf("OK envoi\n" );
    }

    char repo[64];

    if(recv(clientSocket,repo,sizeof(repo),0)<0){
      printf("Problem recv E\n");
    }else{
      printf("Reponse: %s\n", repo);
    }
    int result;
    if(strcmp(repo,reponse)==0){
      result = 1;
      printf("Réponse correcte\n" );
    }else{
      result = 0;
      printf("Réponse incorrecte\n" );
    }

    if(send(clientSocket,&result,sizeof(result),0)<0){
      printf("Problem result\n");
    }


}

  return EXIT_SUCCESS;

}//main
