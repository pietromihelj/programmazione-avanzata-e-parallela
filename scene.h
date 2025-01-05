// Pietro Mihelj SM3201407

#ifndef _SCENE_H
#define _SCENE_H

#include <stdint.h>
#include <stdio.h>

struct _colours {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} __attribute__((packed));

typedef struct _colours colours;
typedef struct _colours * colours_ptr;



struct _coord{
    float x;
    float y;
    float z;
};

typedef struct _coord coord;
typedef struct _coord * coord_ptr;

struct _scene
{
    FILE * fd;
    coord wiew_point;
    colours sfondo;
    int num_obj;
    coord_ptr sfere;
    float * raggi;
    colours_ptr sfere_col;
};

typedef struct _scene scene;
typedef struct _scene * scene_ptr;

int open_scene(char * path, scene_ptr scene);

colours pixel_colour(scene_ptr scena, colours p_col, int x, int y, int widht, int heigth);

#endif