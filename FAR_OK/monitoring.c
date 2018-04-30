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
#define PORT 10059

int sock;
struct Monitoring monitor;

void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(sock, 2);
  close(sock);
  exit(0);
}

void afficheMonitoring(){
  printf("Position oedipe : X %d  Y %d\n", monitor.oedipe.x, monitor.oedipe.y);
  printf("Position Sphynx : X %d  Y %d\n", monitor.sphynx.x, monitor.sphynx.y);
  printf("Position Mercenaire %d : X %d Y %d\n", monitor.mercenaires[0].id, monitor.mercenaires[0].positions.x, monitor.mercenaires[0].positions.y);
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
    fgets(buffer, 32, stdin);
    char *pos = strchr(buffer, '\n');
    *pos = '\0';
    send(sock, buffer, 32, 0);
  //}//for

  //recv(sock, buffer, 32, 0);
  //printf("Recu ID : %s\n", buffer);






  while(1){
    if(recv(sock,(void*)&monitor,sizeof(monitor),0)<0){
      printf("Problem reception Monitoring\n");
    }
    afficheMonitoring();
    sleep(0.01);

  }

  return EXIT_SUCCESS;

}//main
