// Pietro Mihelj SM3201407

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "ppm.h"

int create_immage(char * path, ppm_ptr img, int widht, int heigth){
/*input: nome immagine, struttura da popolare e dimensioni immagine
la funzione crea il file immagine poi popola l'intestazione del file e la struttura. la parte dei 
dati sarà popolata successivamente
ritorna un valore negativo se ci sono stati errori
*/
//creazione file e inserimente intestazione
FILE * fd = fopen(path, "w+");
if (fd == NULL) {
    return -1;
}
int written = fprintf(fd, "P6\n%d %d\n255\n", widht, heigth);
if(ftruncate(fileno(fd), written+ widht * heigth * 3) != 0){
  return -1;
}
fclose(fd);

//lettura file e popolamento struttura
img->fd = fopen(path, "r+");
struct stat sbuff;
stat(path, &sbuff);
img->size = sbuff.st_size;
if(fscanf(img->fd,"P6\n%d %d\n255\n", &img->widht, &img->heigth)!=2){
   fclose(img->fd);
   return -1;
}
img->offset = ftell(img->fd);
//per evitare wrap around dei pixel vado a impostare il mapping di data da dopo l'intestazione, 
//per farlo passo da un casting a uint8_t prima di poter usare il tipo colours_ptr che voglio
img->data = (colours_ptr)((uint8_t*)mmap(NULL, img->size, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(img->fd), 0) + img->offset);
if(img->data == MAP_FAILED) 
{
  fclose(img->fd);
  return -1;
}
return 0;
}


colours_ptr pixel_at(ppm_ptr img, int x, int y){
//input: puntatore all'immagine e coordinate pixel
//la funzione trova il punto della memoria dove inizia il pixel specifico e lo ritorna come puntatore
  if(img == NULL || x>=img->widht || y >= img->heigth || x<0 || y<0){
  return NULL;
  }
  return img->data + (y*img->widht + x);
}


int close_image(ppm_ptr img)
{
  if(img == NULL){
  return -1;
  }
  munmap(img->data, img->size);
  fclose(img->fd);
  return 0;
}