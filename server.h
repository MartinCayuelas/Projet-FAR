#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_MAX 1024
typedef struct requete{
	char requete[BUFFER_MAX];
  int size;
} request;

typedef struct Enigme {
    int TAILLE_ENONCE;
    int TAILLE_REPONSE;
    char* enonce;
    char* reponse;
    int resolue;
}Enigme;


typedef struct Partie{
	int equipe1;
	int equipe2;
	int score1;
	int score2;
}Partie;


typedef struct position{
  int x;
  int y;
}position;

typedef struct objectPosition{
  int nbVillageois;
  position mercenaire[6];
  position oedipe;
  position thebes;
} objectPosition;

typedef struct map {
  int matrice [BUFFER_MAX][BUFFER_MAX];
  int tailleCarte;
  objectPosition positions;
}map;

typedef struct Mercenaire{
	int posX;
	int posY;
	int id;
	int mort;
	int porte;
	int score;
} Mercenaire;

typedef struct Monitoring{
	Mercenaire mercenaires[6];
	position oedipe;
	position sphynx;
	Partie partie;
}Monitoring;
#ifdef __cplusplus
}
#endif
