#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRAVITY 9.81 // Accélération gravitationnelle (m/s²)
#define PI 3.14159265359

typedef struct
{
    double x, y;   // Position
    double vx, vy; // Vitesse
    double mass;   // Masse
} Object;

void init_object(Object *obj, double x, double y, double vx, double vy, double mass)
{
    obj->x = x;
    obj->y = y;
    obj->vx = vx;
    obj->vy = vy;
    obj->mass = mass;
}

int main()
{
    Object *point1 = malloc(sizeof(Object));
    if (point1 == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        return 1;
    }
    init_object(point1, 0, 0, 0, 0, 10);
    printf("x: %f,y : %f\n", point1->x, point1->y);
    free(point1);
    return 0;
}
