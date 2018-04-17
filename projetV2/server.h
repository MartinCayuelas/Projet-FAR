#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER 1024

typedef struct requete{
  int size;
	char request[BUFFER];
}requete;

typedef struct enigme{
	int sizeEnigme;
	int sizeRep;
  char enigme[BUFFER];
  char reponse[BUFFER];

} enigme;


 typedef struct position{
  int x;
  int y;
} position;

typedef struct objectPosition{
  int nbVillageois;
  position mercenaire[6];
  position oedipe[1];
  position thebes[1];
} objectPosition;

typedef struct map {
	int tailleCarte;
  int matrice [BUFFER][BUFFER];

  objectPosition positions;
}map;

#ifdef __cplusplus
}
#endif
