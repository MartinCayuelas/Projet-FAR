// $Id$

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#define PORT 10038

int sock;

void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(sock, 2);
  close(sock);
  exit(0);
}

int main(void) {
  struct sockaddr_in sin;

  /* Rattraper le Ctrl-C */
  signal(SIGINT, close_exit);

  /* Création de la socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration de la connexion */
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);

  /* Tentative de connexion au serveur */
  connect(sock, (struct sockaddr*)&sin, sizeof(sin));
  printf("Connexion a %s sur le port %d\n", inet_ntoa(sin.sin_addr),
         htons(sin.sin_port));

  /* Réception de données du serveur */
  char buffer[32] = "";
  recv(sock, buffer, 32, 0);
  printf("Recu : %s\n", buffer);

  //for(;;) {
    /* Envoi de données au serveur */
    printf("Donnees a envoyer au serveur : ");
    fgets(buffer, 64, stdin);
    char *pos = strchr(buffer, '\n');
    *pos = '\0';
    send(sock, buffer, 64, 0);
  //}//for

  recv(sock, buffer, 32, 0);
  printf("Recu ID : %s\n", buffer);

  struct position mesPositions;

  while(1){
      printf("Votre requete? : ");
      fgets(buffer, 64, stdin);
      char *pos2 = strchr(buffer, '\n');
      *pos2 = '\0';
      send(sock, buffer, 64, 0);

      if(recv(sock,(void*)&mesPositions,sizeof(mesPositions),0) < 0){
        printf("Failed recv\n");
      }

      printf("x: %d\n",mesPositions.x);
      printf("y: %d\n",mesPositions.y);
    }

  return EXIT_SUCCESS;

}//main
