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
#include <signal.h>


#include <time.h>  //pour le Random
#include "server.h"

#define PORT 10801
#define PORTMAP 12030

  int serverSocket;
void close_exit (int n) {
  /* Fermeture de la socket client */
  shutdown(serverSocket, 2);
  close(serverSocket);
  exit(0);
}
//Villageois -> 4
//Thebes -> 8
//oedipe -> 7
//Mercenaire 1 -> 1
//Mercenaire 2 -> 2
//Mercenaire 3 -> 3 etc...
//Sphynx -> 9
// Mur et Eau -> 0



int nbMercenaire = 0;

struct map mapDebut;


int *modificationPosition(char *tokenOrientation, int x, int y){

  printf("x modif AVnt: %d\n",x );
  printf("y modif Ava,t: %d\n",y );
static int tab[2];
  if(strcmp(tokenOrientation,"up")==0){
      tab[1] = y +1;

  }else if(strcmp(tokenOrientation,"down")==0){
  tab[1] = y - 1;
  }else if(strcmp(tokenOrientation,"left")==0){
    tab[0] = y -1;
  }else if(strcmp(tokenOrientation,"right")==0){
      tab[0] = y +1;
  }

  return tab;
}

int rand_a_b(int a, int b){//Fonction de Random
  b+=1;
  return rand()%(b-a) +a;

}




void* clientFils(void *monThreadClient) {

  int csock = *(int*)monThreadClient;

  struct map maMap = mapDebut;


  printf("maMap Mercenaire tailleCarte : %d\n", maMap.tailleCarte);
  struct position positionE;

  positionE.x=0;
  positionE.y=0;

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

          printf("tailleCarte : %d\n", maMap.tailleCarte);

      if(send(csock,(void*)&maMap,sizeof(maMap),0)<0){
        printf("Probleme envoi Map\n");
      }else{
        printf("Map envoyée avec succès au Mercenaire %s\n", idType);
      }

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

            printf("old x: %d\n",positionE.x);
            printf("old y: %d\n",positionE.y);
            int *tab;

            printf("Je suis là\n");
            tab = modificationPosition(tokenOrientation,positionE.x, positionE.y);
printf("Je suis là2\n");
printf("tab[0] %d\n", tab[0]);
            positionE.x = tab[0];
            printf("Je suis là3\n");
            positionE.y = tab[1];
            printf("Je suis là4\n");
            printf("new x: %d\n",positionE.x);
            printf("new y: %d\n",positionE.y);

            int buffer = tab[0];
            int bufferY = tab[1];
            if(send(csock,&buffer,sizeof(buffer),0)<0){
              printf("send  X failed!\n");
            }
            if(send(csock,&bufferY,sizeof(bufferY),0)<0){
              printf("send  Y failed!\n");
            }
            /*
            //Probleme à la reception
            if( send(csock, (void*)&positionE, sizeof(positionE),0) < 0 ) {
                printf("send failed!\n");
            }*/

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

  /****Connexion au serveur de Map ***************/

  struct sockaddr_in mapsin;
  int mapSocket;
  socklen_t maprecsize = sizeof(mapsin);

  /* Création de la socket */
  mapSocket = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration de la connexion */
  mapsin.sin_addr.s_addr = inet_addr("127.0.0.1");
  mapsin.sin_family = AF_INET;
  mapsin.sin_port = htons(PORTMAP);

  /* Tentative de connexion au serveur */
  connect(mapSocket, (struct sockaddr*)&mapsin, sizeof(mapsin));
  printf("Connexion a %s sur le port %d\n", inet_ntoa(mapsin.sin_addr),
         htons(mapsin.sin_port));

         /* Réception de données du serveur */
  char buffer[32] = "";
  recv(mapSocket, buffer, 32, 0);
  printf("Recu : %s\n", buffer);


  struct map laMap;
  if(recv(mapSocket,(void*)&laMap, sizeof(laMap),0)<0){
    printf("Probleme reception\n");
  }

  int j,i;
  for(i = 0; i < laMap.tailleCarte; ++i)
{
   for(j = 0; j < laMap.tailleCarte ; ++j)
   {
        //laMap.matrice[i][j]=0;
      printf("%d\t",  laMap.matrice[i][j]);
   }
printf("\n");
}


struct objectPosition objet;
if(recv(mapSocket,(void*)&objet, sizeof(objet),0)<0){
  printf("Probleme reception\n");
}



printf("Mercenaire 3 x: %d\n",objet.mercenaire[2].x );
printf("Mercenaire 3 y: %d\n",objet.mercenaire[2].y );
printf("Nb nbVillageois: %d\n",objet.nbVillageois );

printf("Je suis ici 1\n");



//placementVillageois(objet.nbVillageois,(int**)tab);
//Verif ajout Villageois
printf("Je suis ici 3\n");


for(i = 0; i< objet.nbVillageois; i++){
  int pose = 0;
    while(pose == 0){
      int positionX = rand_a_b(0,laMap.tailleCarte);
      int positionY = rand_a_b(0,laMap.tailleCarte);
      if(laMap.matrice[positionX][positionY]!=0){
        laMap.matrice[positionX][positionY] = 4;
        pose = 1;
      }
   }
}

for(i = 0; i < laMap.tailleCarte; ++i)
{
 for(j = 0; j < laMap.tailleCarte ; ++j)
 {
      //laMap.matrice[i][j]=0;
    printf("%d\t",  laMap.matrice[i][j]);
 }
printf("\n");
}
/* TEST de carte ******************

if(send(mapSocket,(void*)&laMap, sizeof(laMap),0)<0){
  printf("Probleme Envoi  llaMap\n");
}else{
  printf("OK Envoi MAP MAP\n");
}*/
close(mapSocket);


mapDebut = laMap;

printf("mapDebut tailleCarte: %d\n", mapDebut.tailleCarte);

  /*************************PARTIE DE JEU*****************************/
int cpt = 1;

  while (cpt < 8) {
    /* Attente d'une connexion client */

    if(clientSocket = accept(serverSocket,(struct sockaddr*)&csin, &crecsize)){
    printf("Un client est connecté avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
    pthread_create(&thread, NULL, clientFils, (void *)&clientSocket);
    pthread_detach(thread);


    int poseM = 0;
      while(poseM == 0){
        int positionXm = rand_a_b(0,laMap.tailleCarte);
        int positionYm = rand_a_b(0,laMap.tailleCarte);
        if(laMap.matrice[positionXm][positionYm]== 11){
          laMap.matrice[positionXm][positionYm] = i;
          poseM = 1;
        }
     }
     cpt++;


  }
    /* Rattraper le Ctrl-C */
    signal(SIGINT, close_exit);

  }

  /* Fermeture de la socket serveur */
  close(serverSocket);

  return EXIT_SUCCESS;

}
