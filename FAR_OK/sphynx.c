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
#define PORT 13000

int sock;
struct Enigme e;

void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(sock, 2);
  close(sock);
  exit(0);
}

void afficheEnigme(){
  printf("Enonce : %s\n", e.enonce );
  printf("Reponse: %s\n", e.reponse);
}

int main(void) {
  printf("Sphynx en attente\n");
  e.enonce ="Qui suis je?";
  e.reponse = "Moi";
    afficheEnigme();

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

  clientSocket = accept(sock,(struct sockaddr*)&csin, &crecsize);
  printf("Le serveur de JEU est connecté pour une enigme avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

  /* Réception de données du serveur */

  afficheEnigme();
    if(send(clientSocket,&e,sizeof(e),0)<0){
      printf("Problem send E\n");
    }




  return EXIT_SUCCESS;

}//main
