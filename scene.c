// Pietro Mihelj SM3201407

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "scene.h"

int open_scene(char * path, scene_ptr scena){
//input: il path al file e un puntatore alla struttura da popolare
//la funzione prende il path e inizia a leggere il file popolando la struttura. se vengono trovati errori
//di formattazione vengono segnalati e viene ritornato un valore negativo. se tutto funziona bene viene tornato 0
scena->fd = fopen(path, "r");
if(scena->fd == NULL){
    printf("Errore apertura file\n");
    return -1;
}

if(fscanf(scena->fd,"VP  %f %f %f\nBG %hhu %hhu %hhu\nOBJ_N %d\n",&scena->wiew_point.x,&scena->wiew_point.y,&scena->wiew_point.z, &scena->sfondo.red, &scena->sfondo.green, &scena->sfondo.blue, &scena->num_obj)!=7){
    printf("Intestazione mal formattata\n");
    fclose(scena->fd);
    return -1;
}

//inizzializzazione array da mettere nella scena
scena->sfere = (coord_ptr)malloc(scena->num_obj * sizeof(coord));
scena->raggi = (float *)malloc(scena->num_obj * sizeof(float));
scena->sfere_col = (colours_ptr)malloc(scena->num_obj * sizeof(colours));
for(int i = 0; i<scena->num_obj; i++){
    if(fscanf(scena->fd,"S %f %f %f %f %hhu %hhu %hhu\n",&scena->sfere[i].x,&scena->sfere[i].y ,&scena->sfere[i].z, &scena->raggi[i], &scena->sfere_col[i].red, &scena->sfere_col[i].green, &scena->sfere_col[i].blue)!=7){
        printf("Dati sfere mal formattati\n");
        fclose(scena->fd);
        return -1;
}
}
return 0;
}

colours pixel_colour(scene_ptr scena, colours p_col, int i, int j, int widht, int heigth){
//input: puntatore all'oggetto scena, un blocco di colori, la posizione del pixel, le dimensioni dell'immagine
//la funzione calcola il vettore corrispondente al raggio passante per il pixel e controlla che si incroci o meno 
//con uno degli oggetti descritti nella scena
//ritorna poi il blocco di colori da assegnare al pixel
float min_dist = INFINITY;
coord V;
V.x = (((scena->wiew_point.x)/(widht-1)))*i-(scena->wiew_point.x/2);
V.y = (((scena->wiew_point.y)/(heigth-1)))*j-(scena->wiew_point.y/2);
V.z = 1;
float norma = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
V.x = V.x/norma;
V.y = -V.y/norma; //la componente y deve essere specchiata
V.z = V.z/norma;
float a = V.x*V.x + V.y*V.y + V.z*V.z;

for(int k = 0; k<scena->num_obj; k++){
    float b = -2 * (V.x*scena->sfere[k].x + V.y*scena->sfere[k].y + V.z*scena->sfere[k].z);
    float c = (scena->sfere[k].x*scena->sfere[k].x + scena->sfere[k].y*scena->sfere[k].y + scena->sfere[k].z*scena->sfere[k].z) - (scena->raggi[k]*scena->raggi[k]);
    float det = (b*b) - (4*a*c);
    //printf("%f\n", det);
   if (det >= 0) {
        // Calcolo delle due soluzioni
        float t1 = (-b - sqrt(det)) / (2 * a);

        // Considera solo l'intersezione più vicina valida
        if (t1 > 0 && t1 < min_dist) {
            min_dist = t1;
            p_col = scena->sfere_col[k]; 
        }
}
}
return p_col;
}