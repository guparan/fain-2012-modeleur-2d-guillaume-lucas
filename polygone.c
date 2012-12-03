#include <stdio.h>
#include "polygone.h"
#include "segment.h"

Polygone* polygone_creer()
{
    Polygone* p = (Polygone*) malloc(sizeof(Polygone));
    p->sommets = liste_creer();
    p->ferme = faux;
    p->pmin = point(-1, -1);
    p->pmax = point(-1, -1);
    return p;
}


void polygone_detruire(Polygone *p)
{
    liste_detruire(p->sommets);
    free(p);
}


void polygone_ajouterSommet(Polygone* polygone, Point p)
{
    if(point_sontEgaux(polygone->pmax, point(-1,-1))) // initialisation
    {
        polygone->pmin = p;
        polygone->pmax = p;
    }
    else
    {
        if(p.x > polygone->pmax.x) polygone->pmax.x = p.x + 1;
        if(p.x < polygone->pmin.x) polygone->pmin.x = p.x - 1;
        if(p.y > polygone->pmax.y) polygone->pmax.y = p.y + 1;
        if(p.y < polygone->pmin.y) polygone->pmin.y = p.y - 1;
    }

    liste_insere(polygone->sommets, p);
}


void polygone_selectionneSommetSuivant()
{
    
}


void polygone_remplirGraine(Point pgraine)
{
    int xgraine = pgraine.x, ygraine = pgraine.y;
    Couleur c = val_point(xgraine, ygraine);
    
    if(c == BLANC || c == ROUGE ) return;
    
    if( xgraine < 0 || xgraine > hauteur_ecran() || ygraine < 0 || ygraine > largeur_ecran() )
    {
        return;
    }
    
    change_point(xgraine,ygraine,ROUGE);
    polygone_remplirGraine(point(xgraine-1, ygraine));
    polygone_remplirGraine(point(xgraine, ygraine-1));
    polygone_remplirGraine(point(xgraine+1, ygraine));
    polygone_remplirGraine(point(xgraine, ygraine+1));
}


void polygone_remplirScanline(Polygone* p, Couleur couleur)
{
//    change_point(p->pmin.x, p->pmin.y, couleur);
//    change_point(p->pmax.x, p->pmax.y, couleur);

    int xmin = p->pmin.x, xmax = p->pmax.x, ymin = p->pmin.y, ymax = p->pmax.y;
    int x, y;
    int interieur;
    Point entree;

    if(!p->ferme) return;

    for (y = ymin; y < ymax; y++)
    {
        interieur = 0;
        for (x = xmin; x < xmax; x++)
        {
            if (val_point(x, y) == BLANC && val_point(x+1, y) == NOIR)
            {
                interieur = (interieur+1)%2;
                entree.x = x+1;
                entree.y = y;
            }
            else if (val_point(x, y) == NOIR && interieur)
            {
                change_point(x, y, couleur);
            }
        }
        if(interieur) // effacement d'erreur
        {
            segment_tracerSegmentHorizontal(entree, point(x, y), NOIR);
        }
    }
}




void polygone_remplirScanline2(Polygone* p, Couleur couleur)
{
//    change_point(p->pmin.x, p->pmin.y, couleur);
//    change_point(p->pmax.x, p->pmax.y, couleur);

    int xmin = p->pmin.x, xmax = p->pmax.x, ymin = p->pmin.y, ymax = p->pmax.y;
    int x, y;
    int interieur, deja_colorie;
    Point entree;

    if(!p->ferme) return;

    deja_colorie = 0;
    for (y = ymin; y < ymax; y++)
    {
        interieur = 0;
        for (x = xmin; x < xmax; x++)
        {
            if (val_point(x, y) == NOIR && interieur)
            {
                change_point(x, y, couleur);
                deja_colorie = 1;
            }
            else if (val_point(x, y) == BLANC && val_point(x+1, y) == BLANC)
            {
                interieur = 1;
                entree.x = x+1;
                entree.y = y;
            }
            else if (val_point(x, y) == BLANC && val_point(x+1, y) == NOIR && deja_colorie)
            {
                interieur = (interieur+1)%2;
                entree.x = x+1;
                entree.y = y;
            }
            else if (val_point(x, y) == NOIR && val_point(x+1, y) == BLANC)
            {
                interieur = 0;
            }
        }
        if(interieur) // effacement d'erreur
        {
            segment_tracerSegmentHorizontal(entree, point(x, y), NOIR);
        }
    }
}
