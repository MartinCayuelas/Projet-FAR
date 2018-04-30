
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
	position villageois[8];
} objectPosition;

typedef struct map {
  int matrice [BUFFER_MAX][BUFFER_MAX];
  int tailleCarte;
  objectPosition positions;
}map;

typedef struct Mercenaire{
position positions;
int score;
int connecte;
int mort;
int tientVillageois;
int id;
} Mercenaire;

typedef struct Monitoring{
	Mercenaire mercenaires[6];
	position villageois[8];
	position oedipe;
	position sphynx;
	Partie partie;
}Monitoring;
