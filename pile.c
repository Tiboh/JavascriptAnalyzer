// Stack implementation in C in French
// Found on http://dept-info.labri.fr/~baudon/InitProg/src/pile/3/

#include <assert.h>
#include <stdlib.h>
#include "pile.h"

struct pile {
  int sommet;
  int taille;
  int *pile;
};

#define TAILLE_INITIALE 10

static void 
ajuster(pile p);


pile
pile_creer(void)
{
  pile p = (pile) malloc(sizeof(struct pile));
  assert(p != NULL);
  p->sommet = -1;
  p->taille = TAILLE_INITIALE;
  p->pile = (int *) malloc(p->taille * sizeof(int));
  return p;
}


void
pile_detruire(pile p)
{
  free(p->pile);
  free(p);
}


bool
pile_vide(pile p)
{
  return p->sommet == -1;
}


static void
ajuster(pile p)
{
  if (p->sommet == p->taille - 1)
    {
      p->taille = 2 * p->taille + 1;
      p->pile = realloc(p->pile, p->taille);
    }
  assert(p->pile != NULL);
}


void
pile_empile(pile p, int valeur)
{  
  ajuster(p);
  p->pile[++p->sommet] = valeur;
}


int 
pile_depile(pile p)
{
  int valeur = pile_valeur(p);

  p->sommet--;
  return valeur;
}
  

int 
pile_valeur(pile p)
{
  assert(! pile_vide(p));
  return p->pile[p->sommet];
}


int 
pile_taille(pile p)
{
  return p->sommet + 1;
}