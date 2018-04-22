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

#include <signal.h>


#include <time.h>  //pour le Random
#define PORT 10059

#define PORTMAP 12070

/* VAriables globales pour les fonctions */
int nbMercenaire = 0;

struct map mapJeu; // Pour la Map
struct Partie partie; // Pour la partie
struct Monitoring monitor;
struct position sphynx;


/***********************************************/
int rand_a_b(int a, int b) { //Fonction de Random
  b += 1;
  return rand() % (b - a) + a;

}

void placementVillageois(){
  int i;
  for (i = 0; i < mapJeu.positions.nbVillageois; i++) {
    int pose = 0;
    while (pose == 0) {
      int positionX = rand_a_b(0, mapJeu.tailleCarte);
      int positionY = rand_a_b(0, mapJeu.tailleCarte);
      if (mapJeu.matrice[positionX][positionY] == 10) {
        mapJeu.matrice[positionX][positionY] = 9;
        pose = 1;
      }
    }
  }
}

void placementMOT(){
  int j;
  for(j = 0; j < 6; j++){ //Mercenaire
      int positionX = mapJeu.positions.mercenaire[j].x;
      int positionY = mapJeu.positions.mercenaire[j].y;
      printf("Mercenaire %d X : %d Y : %d\n",j+1,positionX,positionY);
      mapJeu.matrice[positionX][positionY] = j+1;
    }

  mapJeu.matrice[mapJeu.positions.thebes.x][mapJeu.positions.thebes.y] = 33;//Thèbes
  mapJeu.matrice[mapJeu.positions.oedipe.x][mapJeu.positions.oedipe.y] = 7; //Oedipe
  mapJeu.matrice[mapJeu.tailleCarte-1][mapJeu.tailleCarte-1] = 8; //Sphynx
  sphynx.x = mapJeu.tailleCarte-1;
  sphynx.y = mapJeu.tailleCarte-1;
}

void resetPlacement(int x, int y){
    mapJeu.matrice[x][y] = 10;
}

void setEquipe(int i, int mort){
  if(i <= 3){
    if( mort == 0){
      partie.equipe1 += 1;
    }else{
      partie.equipe1 -= 1;
    }

  }else{
    if( mort == 0){
      partie.equipe2 += 1;
    }else{
      partie.equipe2 -= 1;
    }
  }
}
void setScore(int i){
  if(i <= 3){
    partie.score1 += 1;
  }else{
    partie.score2 += 1;
  }
}

void attaquer(int x, int y){
  int xOedipe = mapJeu.positions.oedipe.x;
  int yOedipe = mapJeu.positions.oedipe.y;

  int idMercenaire = mapJeu.matrice[x][y];
  setEquipe(idMercenaire,1);
  resetPlacement(x,y);
  mapJeu.positions.oedipe.x = x;
  mapJeu.positions.oedipe.y = y;
  placementMOT();


}

void afficheMap(){
    int i,j;
    for(i = 0; i < mapJeu.tailleCarte; ++i){
     for(j = 0; j < mapJeu.tailleCarte ; ++j){
        printf("%d\t",mapJeu.matrice[i][j]);
     }
     printf("\n");
   }
}

int verifMur(int x, int y){
  int mur = 0;
  if(mapJeu.matrice[x][y]==0){
    mur = 1;
  }

  return mur;
}



void afficheMercenaire(Mercenaire mercenaire){
  printf("Id: %d\n", mercenaire.id);
  printf("X: %d\n", mercenaire.posX);
  printf("Y: %d\n", mercenaire.posY);
  printf("Mort? : %d\n", mercenaire.mort);
  printf("Porte? : %d\n", mercenaire.porte);
  printf("Score? : %d\n", mercenaire.score);
}

void modificationPosition(char * tokenOrientation,char * id) {
    int idT = atoi(id);
    int oldX;
    int oldY;
    int mur;

    if(idT != 7){

      oldX = mapJeu.positions.mercenaire[idT-1].x;
      oldY = mapJeu.positions.mercenaire[idT-1].y;

    if (strcmp(tokenOrientation, "up") == 0) {

      mapJeu.positions.mercenaire[idT-1].y += 1;
        monitor.mercenaires[idT-1].posY += 1; //MONITOR
       mur = verifMur(mapJeu.positions.mercenaire[idT-1].x, mapJeu.positions.mercenaire[idT-1].y);
    } else if (strcmp(tokenOrientation, "down") == 0) {
    mapJeu.positions.mercenaire[idT-1].y -= 1;
    monitor.mercenaires[idT-1].posY -= 1; //MONITOR
    mur = verifMur(mapJeu.positions.mercenaire[idT-1].x, mapJeu.positions.mercenaire[idT-1].y);
    } else if (strcmp(tokenOrientation, "left") == 0) {
      mapJeu.positions.mercenaire[idT-1].x -= 1;
      monitor.mercenaires[idT-1].posX -= 1; //MONITOR
      mur = verifMur(mapJeu.positions.mercenaire[idT-1].x, mapJeu.positions.mercenaire[idT-1].y);
    } else if (strcmp(tokenOrientation, "right") == 0) {
    mapJeu.positions.mercenaire[idT-1].x += 1;
    monitor.mercenaires[idT-1].posX += 1; //MONITOR
    mur = verifMur(mapJeu.positions.mercenaire[idT-1].x, mapJeu.positions.mercenaire[idT-1].y);
    }
  }else{
     oldX = mapJeu.positions.oedipe.x;
     oldY = mapJeu.positions.oedipe.y;
    if (strcmp(tokenOrientation, "up") == 0) {
      mapJeu.positions.oedipe.y += 1;

      monitor.oedipe.y += 1;//TEST Monitoring

      mur = verifMur( mapJeu.positions.oedipe.x,mapJeu.positions.oedipe.y);

    } else if (strcmp(tokenOrientation, "down") == 0) {
    mapJeu.positions.oedipe.y -= 1;
    monitor.oedipe.y -= 1;//TEST Monitoring
      mur = verifMur(  mapJeu.positions.oedipe.x,mapJeu.positions.oedipe.y);
    } else if (strcmp(tokenOrientation, "left") == 0) {
      mapJeu.positions.oedipe.x -= 1;
      monitor.oedipe.x -= 1;//TEST Monitoring
        mur = verifMur(  mapJeu.positions.oedipe.x,mapJeu.positions.oedipe.y);
    } else if (strcmp(tokenOrientation, "right") == 0) {
      mapJeu.positions.oedipe.x += 1;
      monitor.oedipe.x += 1;//TEST Monitoring
        mur = verifMur(  mapJeu.positions.oedipe.x,mapJeu.positions.oedipe.y);
    }
  }

    if(mur == 0){
      resetPlacement(oldX,oldY); // On enleve la position ancienne
      placementMOT();
    }else{
        if(idT == 7){
            mapJeu.positions.oedipe.x = oldX;
            mapJeu.positions.oedipe.y = oldY;
            monitor.oedipe.x = oldX;//TEST Monitoring
            monitor.oedipe.y = oldY;//TEST Monitoring



        }else{
            mapJeu.positions.mercenaire[idT-1].x = oldX;
            mapJeu.positions.mercenaire[idT-1].y = oldY;
            monitor.mercenaires[idT-1].posX = oldX;
            monitor.mercenaires[idT-1].posY = oldY;

        }
        placementMOT();
    }
}

void * clientFils(void * monThreadClient) {

  int csock = * (int *) monThreadClient;

  struct Mercenaire mercenaire;


  //----------Partie Debut----------------//
  char bufferC[32] = "Bonjour!";
  send(csock, bufferC, 32, 0);

  //----------Partie Connexion----------------//
  char bufferConnexion[128];
  recv(csock, bufferConnexion, 128, 0);
  printf("%s\n", bufferConnexion);

  //Savoir quel type se connecte
  char * tokenConnexion = strtok(bufferConnexion, "/");
  printf("%s\n", tokenConnexion);
  if (strcmp(tokenConnexion, "connexion") == 0) {
    printf("Demande de connexion Possible\n");

    tokenConnexion = strtok(NULL, "");
    char * tokenType = strtok(tokenConnexion, "/");

    printf("%s\n", tokenType);

    if (strcmp(tokenConnexion, "mercenaire") == 0) {
      nbMercenaire = nbMercenaire + 1; // On incrémente le nombre de Mercenaire
      setEquipe(nbMercenaire,0); // Augmentation du nombre de joueur dans l'équipe concernée
      printf("NB joueurs Equipe1 %d\n", partie.equipe1);
      printf("NB joueurs Equipe2 %d\n", partie.equipe2);
      char idType[1];
      sprintf(idType, "%d", nbMercenaire);
      printf("Id: %s\n", idType);
      int idTypeInt = atoi(idType);

      mercenaire.id = idTypeInt;
      mercenaire.posX = mapJeu.positions.mercenaire[mercenaire.id-1].x;
      mercenaire.posY = mapJeu.positions.mercenaire[mercenaire.id-1].y;
      mercenaire.mort = 0;
      mercenaire.porte = 0;
      mercenaire.score = 0;

      monitor.mercenaires[idTypeInt-1] = mercenaire;

      afficheMercenaire(mercenaire);
      //send(csock, idType, sizeof(idType), 0);
      if(send(csock,(void*)&mercenaire,sizeof(mercenaire),0)<0){
        printf("Probleme mercenaire ENvoi\n");
      }
      /*if(send(csock,(void*)&mapJeu,sizeof(mapJeu),0)<0){
        printf("Probleme Map Envoi\n");
      }*/

    } else if (strcmp(tokenConnexion, "oedipe") == 0) {
      //TO DO
      char * idType = "7";

      printf("Id: %s\n", idType);
      send(csock, idType, sizeof(idType), 0);
      /* code */
    } else if (strcmp(tokenConnexion, "sphynx") == 0) {
      //TO DO
      char * idType = "8";
      printf("Id: %s\n", idType);
      send(csock, idType, sizeof(idType), 0);
      /* code */
    } else if(strcmp(tokenConnexion,"monitoring") == 0){
      while (1) {

        if(send(csock,(void*)&monitor, sizeof(monitor),0)<0){
          printf("Problem send data to monitoring\n");
        }

      }
      printf("Monitoring terminé\n");
      close(csock);

    } else {
    char badRequest[64] = "Mauvaise forme de requete de connexion!\n";
    printf("%s\n", badRequest);
    send(csock, badRequest, 64, 0);
    }
  } // Fin Connexion

  //--------------Partie Traitement------------//
  //char bufferAccueil[32] = "\nBonjour! et Bienvenue\n";
  //send(csock, bufferAccueil, 32, 0);

  /********** ENvoi de la Map*************/
  /*
  int socketMap = csock;
  if (send(socketMap, (void *)&mapJeu, sizeof(mapJeu), 0) < 0) {
    printf("Probleme envoi MAP\n");
  } else {
    printf("Envoyé MAP au client\n");
  }

  close(socketMap);*/
/*****************************************************/
  char action[64] = "";
  while (recv(csock, action, 64, 0) > 0) {

    printf("Action : %s\n", action);

    //Recuperer l'action
    char * tokenId = strtok(action, "/");
    printf("%s\n", tokenId);
    char id[1];
    strcpy(id, tokenId);
    int monId = atoi(id);

    printf("Mon id : %d\n", monId);
    if (strcmp(tokenId, "8") == 0) {

    } else {
      tokenId = strtok(NULL, "");
      char * tokenAction = strtok(tokenId, "/");
      printf("%s\n", tokenAction);

      if (strcmp(tokenAction, "deplacer") == 0 && (mercenaire.id == monId || monId == 7)) {
        tokenId = strtok(NULL, "");
        char * tokenOrientation = strtok(tokenId, "/");
        printf("%s\n", tokenOrientation);
        if (strcmp(tokenOrientation, "up") == 0) {
          printf("up\n");
        } else if (strcmp(tokenOrientation,"down") == 0) {
          printf("down\n");
        } else if (strcmp(tokenOrientation,"left") == 0) {
          printf("left\n");
        } else if (strcmp(tokenOrientation,"right") == 0) {
          printf("right\n");
        } else { //probleme de requete
          char badRequest[64] = "Mauvaise forme de requete de déplacement!\n";
          printf("%s\n", badRequest);
          send(csock, badRequest, 64, 0);
        }
        modificationPosition(tokenOrientation,id); //
        afficheMap();
        /*
        if (send(csock, (struct position*)&position, sizeof(position), 0) < 0) {
          printf("send failed!\n");
        }*/
        //TO DO
      } else if (strcmp(tokenAction, "attraper") == 0 && mercenaire.id != 7) {
        //TO DO
      } else if (strcmp(tokenAction, "attaquer") == 0 && (strcmp(id, "7") == 0)) {
        //TO DO
      }
    }

  }
  printf("Coucou Je suis plus dans le while\n");

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
  sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
  sin.sin_family = AF_INET; /* Protocole familial (IP) */
  sin.sin_port = htons(PORT); /* Listage du port */
  bind(serverSocket, (struct sockaddr * ) & sin, recsize);

  /* Démarrage du listage */
  listen(serverSocket, 15);
  int sockMap;
  struct sockaddr_in mapsin;

  /************PARTIE MAP********************/
  /* Création de la socket */
  sockMap = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration de la connexion */
  mapsin.sin_addr.s_addr = inet_addr("127.0.0.1");
  mapsin.sin_family = AF_INET;
  mapsin.sin_port = htons(PORTMAP);

  /* Tentative de connexion au serveur */
  connect(sockMap, (struct sockaddr * )&mapsin, sizeof(mapsin));
  printf("Connexion a %s sur le port %d\n", inet_ntoa(mapsin.sin_addr),
    htons(mapsin.sin_port));

  int i, j;
  if (recv(sockMap, (void * ) & mapJeu, sizeof(mapJeu), 0) < 0) {
    printf("Probleme Reception MAp\n");
  } else {

    printf("Coucou j'ai reçu la map\n");
    afficheMap();


    printf("Nb nbVillageois: %d\n", mapJeu.positions.nbVillageois);

    placementMOT();
    placementVillageois();
    afficheMap();


  }

  if (send(sockMap, (void * ) & mapJeu, sizeof(mapJeu), 0) < 0) {
    printf("Probleme envoi MAP\n");
  } else {
    printf("Envoyé MAP\n");
  }

  close(sockMap);

  partie.equipe1 = 0;
  partie.equipe2 = 0;
  partie.score1 = 0;
  partie.score1 = 0;

  monitor.oedipe = mapJeu.positions.oedipe;
  monitor.sphynx = sphynx;
  monitor.partie = partie;

  /********************************************************/
  while (1) {
    /* Attente d'une connexion client */

    clientSocket = accept(serverSocket, (struct sockaddr * ) & csin, & crecsize);
    printf("Un client est connecté avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
    pthread_create( & thread, NULL, clientFils, (void * ) & clientSocket);
    pthread_detach(thread);



  }

  /* Fermeture de la socket serveur */
  close(serverSocket);

  return EXIT_SUCCESS;

}
