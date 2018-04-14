#ifdef __cplusplus
extern "C" {
#endif


struct requete{
	char requete[size];
  int size;
};
typedef struct requete request;

struct enigme{
  char enigme[size];
  char reponse[sizeRep];
  int sizeEnigme;
  int sizeRep;
}
typedef struct enigme enigme;


typedef struct position{
  int x;
  int y;
}position;

typedef struct objectPosition{
  int nbVillageois;
  position mercenaire[6];
  position oedipe[1];
  position thebes[1];
} objectPosition;

typedef struct map {
  int matrice [tailleCarte][tailleCarte];
  int tailleCarte;
  objectPosition positions;
}map;

#ifdef __cplusplus
}
#endif
