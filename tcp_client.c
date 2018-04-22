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
#define PORT 10059

int sock;
struct map mapJeu;
struct position mesPositions;
struct Mercenaire mercenaire;

void afficheMercenaire(Mercenaire mercenaire){
  printf("Id: %d\n", mercenaire.id);
  printf("X: %d\n", mercenaire.posX);
  printf("Y: %d\n", mercenaire.posY);
  printf("Mort? : %d\n", mercenaire.mort);
  printf("Porte? : %d\n", mercenaire.porte);
  printf("Score? : %d\n", mercenaire.score);
}



void afficheMap(map mapJeu){
  printf("Je suis dans afficheMap\n");
  int i,j;

  printf("Taille Carte %d\n",mapJeu.tailleCarte);
  for(i = 0; i < mapJeu.tailleCarte; i++){
   for(j = 0; j < mapJeu.tailleCarte ; j++){
      printf("%d\t",mapJeu.matrice[i][j]);
   }
   printf("\n");
 }
}
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
    fgets(buffer, 32, stdin);
    char *pos = strchr(buffer, '\n');
    *pos = '\0';
    send(sock, buffer, 32, 0);
  //}//for

  //recv(sock, buffer, 32, 0);
  //printf("Recu ID : %s\n", buffer);

  char * tokenConnexion = strtok(buffer, "/");
  printf("%s\n", tokenConnexion);
  if (strcmp(tokenConnexion, "connexion") == 0) {
    printf("Demande de connexion Client\n");

    tokenConnexion = strtok(NULL, "");
    char * tokenType = strtok(tokenConnexion, "/");

    printf("%s\n", tokenType);

    if (strcmp(tokenConnexion, "mercenaire") == 0) {
      recv(sock,(void*)&mercenaire,sizeof(mercenaire),0);
      afficheMercenaire(mercenaire);
    }else if (strcmp(tokenConnexion, "oedipe") == 0){
      char id[32];
      recv(sock,id,sizeof(id),0);
      printf("id : %s\n", id);

    }
  }

  while(1){
    char bufferR[64];
      printf("Votre requete? : ");
      fgets(bufferR, 64, stdin);
      char *pos2 = strchr(bufferR, '\n');
      *pos2 = '\0';
      if(send(sock, bufferR, 64, 0)<0){
        printf("probleme envoi requete\n");
      }
  }

  return EXIT_SUCCESS;

}//main
