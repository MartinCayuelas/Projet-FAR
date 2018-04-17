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
#define PORT 10801

int sock;

void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(sock, 2);
  close(sock);
  exit(0);
}

int main(void) {
  struct sockaddr_in sin;



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
    printf("RequeteConnexion : ");
    fgets(buffer, 32, stdin);
    char *pos = strchr(buffer, '\n');
    *pos = '\0';
    send(sock, buffer, 32, 0);
  //}//for

char bufferId[32] = "";
  recv(sock, bufferId, 32, 0);
  printf("Recu ID : %s\n", bufferId);

  /*definitions des structures*/
  struct map mapJeu;
  struct position mesPositions;


  int sock2Map = sock;
  printf("Preparation du jeu---------------\n");
  printf("/!/ MAP En cours d'édition\n");

  if(recv(sock2Map,(void*)&mapJeu,sizeof(mapJeu),0)<0){
    printf("Probleme Reception map\n");
  }else{//Affichage MAP
    int j,i;

    printf("tailleCarte : %d\n", mapJeu.tailleCarte);


    printf("Matrix[1][1]: %d\n", mapJeu.matrice[1][1]);
    printf("Matrix[3][3]: %d\n", mapJeu.matrice[3][3]);

    for(i = 0; i < mapJeu.tailleCarte; ++i)
  {
     for(j = 0; j < mapJeu.tailleCarte ; ++j)
     {

        printf("%d\t",  mapJeu.matrice[i][j]);
     }
  printf("\n");
  }

  }
 
 close(sock2Map);

  while(1){
      printf("Votre requete? : ");
      fgets(buffer, 64, stdin);
      char *pos2 = strchr(buffer, '\n');
      *pos2 = '\0';

      //Savoir quel type se connecte
      char *tokenRequete= strtok(buffer,"/");
      printf("%s\n",tokenRequete);
      if(strcmp(tokenRequete, bufferId)==0){
          send(sock, buffer, 64, 0);
          /*
          if(recv(sock,(void*)&mesPositions,sizeof(mesPositions),0) < 0){
            printf("Failed recv\n");
          }

          printf("x: %d\n",mesPositions.x);
          printf("y: %d\n",mesPositions.y);*/
          if(strcmp(tokenRequete,"deplacer")==0){
            int bufferX[1];
            int bufferY[1];
            if(recv(sock,bufferX,sizeof(buffer),0)<0){
              printf("recv  X failed!\n");
            }
            if(recv(sock,bufferY,sizeof(bufferY),0)<0){
              printf("recv  Y failed!\n");
            }

            printf("X : %d\n", bufferX[0]);
            printf("Y : %d\n", bufferY[0]);
        }else if(strcmp(tokenRequete,"demande")==0){
            struct enigme monEnigme;

            if(recv(sock,(void*)&monEnigme,sizeof(monEnigme),0)<0){
              printf("Erreur Reception Enigme \n");
            }

        }

        /* Rattraper le Ctrl-C */
        signal(SIGINT, close_exit);
    }else{
      printf("Action Non valide pour votre ID\n" );
    }

    }



  return EXIT_SUCCESS;

}//main
