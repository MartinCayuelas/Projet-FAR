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

#define PORT 18080

int main(void) {
printf("Initialisation du serveur MAP\n");



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
  listen(serverSocket, 1);

  clientSocket = accept(serverSocket,(struct sockaddr*)&csin, &crecsize);
  printf("Le serveur de JEU est connecté avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

  char bufferMap[32];
  if(recv(clientSocket, bufferMap, sizeof(bufferMap),0)<0){
    printf("Problème reception choix Map\n" );
  }else{
    printf("Choix: %s\n", bufferMap);
  }

  struct objectPosition objet;
  struct map laMap;

  laMap.tailleCarte = 10;
  laMap.matrice[laMap.tailleCarte][laMap.tailleCarte];
  int j,i;
  for(i = 0; i < laMap.tailleCarte; ++i)
{
   for(j = 0; j < laMap.tailleCarte ; ++j)
   {
        laMap.matrice[i][j]=10;
      printf("%d\t",  laMap.matrice[i][j]);
   }
printf("\n");
}


laMap.matrice[1][1]= 0;
laMap.matrice[2][1]= 0;
laMap.matrice[3][1]= 0;

laMap.matrice[6][6]= 0;
laMap.matrice[6][7]= 0;

laMap.matrice[8][8]= 0;

laMap.matrice[1][2]= 11;
laMap.matrice[1][3]= 11;
laMap.matrice[1][4]= 11;

struct position oedipe;
struct position mercenaire1;
struct position mercenaire2;
struct position mercenaire3;
struct position mercenaire4;
struct position mercenaire5;
struct position mercenaire6;

struct position thebe;

thebe.x = 8;
thebe.y = 8;
oedipe.x = 1;
oedipe.y = 1;
objet.nbVillageois = 8;
objet.oedipe = oedipe;

mercenaire1.x = 2;
mercenaire1.y = 2;
mercenaire2.x = 3;
mercenaire2.y = 3;
mercenaire3.x = 4;
mercenaire3.y = 4;
mercenaire4.x = 5;
mercenaire4.y = 5;
mercenaire5.x = 6;
mercenaire5.y = 6;
mercenaire6.x = 7;
mercenaire6.y = 7;



objet.mercenaire[0] = mercenaire1;
objet.mercenaire[1] = mercenaire2;
objet.mercenaire[2] = mercenaire3;
objet.mercenaire[3] = mercenaire4;
objet.mercenaire[4] = mercenaire5;
objet.mercenaire[5] = mercenaire6;
objet.thebes = thebe;

laMap.positions = objet;

printf("Coucou avant send\n" );

  if(send(clientSocket,&laMap,sizeof(laMap),0) < 0 ){
    printf("Probleme envoi MAP\n");
  }else{
    printf("Envoyé MAP\n");
  }

printf("Coucou après send\n" );

// TEST de carte ************************
printf("CARTE RETOUR Avec placement villageois + Mercenaire + Oedipe + sphynx\n");
  if(recv(clientSocket,&laMap,sizeof(laMap),0) < 0 ){
    printf("Probleme reception\n");
  }else{
    for(i = 0; i < laMap.tailleCarte; ++i)
  {
     for(j = 0; j < laMap.tailleCarte ; ++j)
     {

        printf("%d\t",  laMap.matrice[i][j]);
     }
  printf("\n");
  }

}
close(clientSocket);
close(serverSocket);



}
