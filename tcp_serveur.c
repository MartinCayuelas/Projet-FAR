// $Id$

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define PORT 10001

void *thread_connect(void *csock_desc) {

  /* Récupération de la socket client */
  int csock = *(int*)csock_desc;

  /* Envoi de données au client */
  char buffer[32] = "Bonjour !";
  send(csock, buffer, 32, 0);

  for(;;) {
    /* Réception de données du client */
    int res = recv(csock, buffer, 32, 0);
    if (res == 0)
      break;
    printf("Recu de la socket %d : %s\n", csock, buffer);
  }//for

  /* Fermeture de la socket dans les deux sens */
  shutdown(csock, 2);

  /* Fermeture de la socket client */
  close(csock);

  return 0;

}//connect

int main(void) {

  /* Socket et contexte d'adressage du serveur */
  struct sockaddr_in sin;
  int sock;
  socklen_t recsize = sizeof(sin);

  /* Socket et contexte d'adressage du client */
  struct sockaddr_in csin;
  int csock;
  socklen_t crecsize = sizeof(csin);

  /* Pour forker le moment venu */
  pid_t pid;

  /* Création d'une socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration */
  sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
  sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
  sin.sin_port = htons(PORT);               /* Listage du port */
  bind(sock, (struct sockaddr*)&sin, recsize);

  /* Démarrage du listage (mode server) */
  listen(sock, 5);

  /* Pour créer les threads */
  pthread_t thread_id;

  /* Créer la thread comme détachée */
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for(;;) {
    /* Attente d'une connexion client */
    csock = accept(sock, (struct sockaddr*)&csin, &crecsize);
    printf("Un client se connecte avec la socket %d de %s:%d\n",
           csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

    /* On crée une thread pour faire le traitement */
    pthread_create(&thread_id, &attr, thread_connect, (void*) &csock);
  }//for

  /* Fermeture de la socket serveur */
  close(sock);

  return EXIT_SUCCESS;

}//main
