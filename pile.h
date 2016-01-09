// Stack implementation in C in French
// Found on http://dept-info.labri.fr/~baudon/InitProg/src/pile/3/

#ifndef PILE_H
#define PILE_H

#include <stdbool.h>

typedef struct pile * pile;

pile
pile_creer();

void
pile_detruire(pile p);

bool
pile_vide(pile p);

/* Empile la valeur passée en paramètre, la taille de la pile étant réajustée si besoin.*/

void
pile_empile(pile p, int valeur);

/* Attention, depiler une pile vide provoque une erreur.
*/
int 
pile_depile(pile p);

/* Renvoie la valeur en sommet de pile. Provoque une erreur si la pile 
   est vide.
*/
int 
pile_valeur(pile p);

/* Renvoie contenue dans la pile à la position  */
int
pile_valeur_position(pile p, int position);

/* Nombre d'éléments dans la pile.
 */
int 
pile_taille(pile p);

#endif