// Pietro Mihelj SM3201407

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include "scene.h"
#include "ppm.h"

int main(int argc, char * argv[]){
/*la main andrà a:
-prendere i dati in input e usarli per creare e popolare le strutture necessarie tramite funzioni apposite
-scorrere sui pixel e assegnargli il colore trovato da funzione apposita
-chiudere le strutture ed il collegamento mmap
*/
// creazione strutture necessarie e presa delle informazioni iniziali
scene scena;
ppm image;
char scene_name[100];
char img_name[100];
int widht;
int heigth;
printf("inserire nome scena: ");
if(scanf("%s", scene_name) != 1){
    printf("errore: bisogna inserie una stringa unica");
    return 0;
}
printf("inserire il nome dell'immagine da creare: ");
if(scanf("%s",img_name) != 1){
    printf("errore: bisogna inserie una stringa unica");
    return 0;
}  
printf("inserire larghezza e altezza del'immagine in pixel nella forma xxx xxx: ");
if(scanf("%d %d",&widht, &heigth) != 2){
    printf("errore: bisogna inserire 2 numeri interi");
}  

//apertura file scena, controllo correttezza e popolamento struttura
int err = open_scene(scene_name, &scena);
if(err!=0){
    return 0;
}
printf("test1 ok\n");

//creazione immagine
err = create_immage(img_name,&image,widht,heigth);
if (err != 0) {
    printf("errore nella crezione dell'oggetto immagine\n");
    return 0;
}
printf("test2 ok\n");
  

//processamento pixel
double start = omp_get_wtime();
#pragma omp parallel for collapse(2)
for(int i = 0; i < image.widht; i++){
    for(int j = 0; j < image.heigth; j++){
        colours p_col;
        //inizzializzo il pixel del colore dello sfondo cosi se non incontra nessun oggetto e non cambierà colore rimarra del colore desiderato
        p_col.red = scena.sfondo.red;   
        p_col.green = scena.sfondo.green;
        p_col.blue = scena.sfondo.blue;
        //creo il pixel come un puntatore ad una struttura colours 
        colours_ptr pixel = pixel_at(&image, i,j);    
        *pixel = pixel_colour(&scena, p_col, i, j, image.widht, image.heigth);
}
}
double end = omp_get_wtime();
double ms = (end - start) * 1000;

printf("test3 ok\n");

err = close_image(&image);
if (err != 0) {
    printf("errore nella chiusura dell'oggetto immagine\n");
    return 0;
}
printf("test4 ok\n");

printf("Time of computation: %f ms\n", ms);


return 0;
}