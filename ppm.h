// Pietro Mihelj SM3201407

#ifndef _PPM_H
#define _PPM_H

#include <stdio.h>
#include <stdint.h>
#include "scene.h"

struct _ppm_image{
    int widht;
    int heigth;
    int offset;
    int size;
    FILE * fd;
    colours_ptr data;
}__attribute__((packed));

typedef struct _ppm_image ppm;
typedef struct _ppm_image * ppm_ptr;

int create_immage(char * path, ppm_ptr img, int widht, int heigth);

colours_ptr pixel_at(ppm_ptr img, int x, int y);

int close_image(ppm_ptr img);
#endif