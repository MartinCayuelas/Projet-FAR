#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>

#include "server.h"

#define PORT 10034


int nbMercenaire = 0;


void modificationPosition(char *tokenOrientation, position *position){

  if(strcmp(tokenOrientation,"up")==0){
    position->y+=1;

  }else if(strcmp(tokenOrientation,"down")==0){
    position->y-=1;
  }else if(strcmp(tokenOrientation,"left")==0){
    position->x-=1;
  }else if(strcmp(tokenOrientation,"right")==0){
      position->x+=1;
  }
}


void* clientFils(void *monThreadClient) {

  int csock = *(int*)monThreadClient;


  int mesPositions[2];

  struct position position;

  position.x=0;
  position.y=0;

  //----------Partie Debut----------------//
  char bufferC[32] = "Bonjour!";
  send(csock, bufferC,32,0);



  //----------Partie Connexion----------------//
  char bufferConnexion[128];
  recv(csock, bufferConnexion,128,0);
    printf("%s\n", bufferConnexion);

  //Savoir quel type se connecte
  char *tokenConnexion = strtok(bufferConnexion,"/");
  printf("%s\n",tokenConnexion);
  if(strcmp(tokenConnexion,"connexion")==0){
    printf("Demande de connexion Possible\n");

    tokenConnexion = strtok(NULL,"");
    char *tokenType = strtok(tokenConnexion,"/");

  printf("%s\n",tokenType);

    if (strcmp(tokenConnexion,"mercenaire")==0) {
      nbMercenaire = nbMercenaire + 1; // On incrémente le nombre de Mercenaire

      char idType[1];
      sprintf(idType, "%d", nbMercenaire);

      printf("Id: %s\n", idType);

      send(csock,idType,sizeof(idType),0);

      /* code */
    }else if (strcmp(tokenConnexion,"oedipe")==0) {
      //TO DO
      char* idType ="7";

        printf("Id: %s\n", idType);
      send(csock,idType,sizeof(idType),0);
      /* code */
    }else if (strcmp(tokenConnexion,"sphynx")==0) {
      //TO DO
        char* idType ="8";
          printf("Id: %s\n", idType);
      send(csock,idType,sizeof(idType),0);
      /* code */
    }


  }else{
    char badRequest[64]= "Mauvaise forme de requete de connexion!\n";
    printf("%s\n", badRequest);
    send(csock, badRequest,64,0);
  }




  //--------------Partie Traitement------------//
  char bufferAccueil[64]="\nBonjour! et Bienvenue\n";
  send(csock,bufferAccueil,64,0);

  char action[64]="";
  while(recv(csock,action,64,0) > 0){
      printf("%s\n",action);

      //Recuperer l'action
      char *tokenId = strtok(action,"/");
      printf("%s\n",tokenId);
      char id[1];
      strcpy(id,tokenId);

      if(strcmp(tokenId,"8")==0){
      }
      else{
        tokenId= strtok(NULL,"");
        char *tokenAction = strtok(tokenId,"/");
        printf("%s\n",tokenAction );

        if(strcmp(tokenAction,"deplacer")==0){
            tokenId= strtok(NULL,"");
          char *tokenOrientation = strtok(tokenId,"/");
          printf("%s\n",tokenOrientation );
            if(strcmp(tokenOrientation,"up")==0){
              printf("up\n");
            }else if(strcmp(tokenOrientation,"down")==0){
              printf("down\n");
            }else if(strcmp(tokenOrientation,"left")==0){
              printf("left\n");
            }else if(strcmp(tokenOrientation,"right")==0){
              printf("right\n");
            }
            else{ //probleme de requete
              char badRequest[64]= "Mauvaise forme de requete de déplacement!\n";
              printf("%s\n", badRequest);
              send(csock, badRequest,64,0);
            }

            printf("old x: %d\n",position.x);
            printf("old y: %d\n",position.y);
            modificationPosition(tokenOrientation,&position);
            printf("new x: %d\n",position.x);
            printf("new y: %d\n",position.y);


            if( send(csock, &position, sizeof(position),0) < 0 ) {
                printf("send failed!\n");
            }

            //TO DO
        }else if(strcmp(tokenAction,"attraper")==0 && (strcmp(id,"7")!=0)){
          //TO DO
        }
        else if(strcmp(tokenAction,"attaquer")==0 && (strcmp(id,"7")==0)){
          //TO DO
        }
      }




  }





  pthread_exit(0);
  close(csock);



}

int main(void) {
printf("Initialisation du serveur\n");

  pthread_t thread;

  /* Socket et contexte d'adressage du serveur */
  struct sockaddr_in sin;
  int serverSocket;
  socklen_t recsize = sizeof(sin);

  /* Socket et contexte d'adressage du client */
  struct sockaddr_in csin;
  int clientSocket;
  socklen_t crecsize = sizeof(csin);

  /* Création d'une socket */
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration */
  sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
  sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
  sin.sin_port = htons(PORT);               /* Listage du port */
  bind(serverSocket, (struct sockaddr*)&sin, recsize);

  /* Démarrage du listage */
  listen(serverSocket, 15);

  while (1) {
    /* Attente d'une connexion client */

    clientSocket = accept(serverSocket,(struct sockaddr*)&csin, &crecsize);
    printf("Un client est connecté avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
    pthread_create(&thread, NULL, clientFils, (void *)&clientSocket);
    pthread_detach(thread);

  }

  /* Fermeture de la socket serveur */
  close(serverSocket);

  return EXIT_SUCCESS;

}
