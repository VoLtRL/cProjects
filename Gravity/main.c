#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GRAVITY 6.6742 * pow(10, -11) // Constante gravitationnelle (m³kg⁻¹s⁻²)
#define PI 3.14159265358979323846
#define RES 100
#define WIDTH 1920
#define HEIGHT 1200
#define DELTA_T 1
#define RESTCOEFF 1
#define CAMERAFAC 5000000

typedef struct
{
    double x, y;   // Position
    double vx, vy; // Vitesse
    double mass;   // Masse
    double radius; // Rayon
} Object;

typedef struct
{
    double x, y;
} Vector2D;

void init_object(Object *obj, double x, double y, double vx, double vy, double mass)
{
    obj->x = x;
    obj->y = y;
    obj->vx = vx;
    obj->vy = vy;
    obj->mass = mass;
}

double distance(Object obj1, Object obj2)
{
    return sqrt(pow(obj2.y - obj1.y, 2) + pow(obj2.x - obj1.x, 2));
}

void calculate_acc(Object obj1, Object obj2, double *acc)
/*
This function calculates acceleration for x and for y axes of obj1
*/
{
    double direction[2];

    double dx = obj2.x - obj1.x;
    double dy = obj2.y - obj1.y;
    direction[0] = dx / distance(obj1, obj2);
    direction[1] = dy / distance(obj1, obj2);

    double Gforce = (GRAVITY * obj1.mass * obj2.mass) / pow(distance(obj1, obj2), 2);
    double a = Gforce / obj1.mass;
    acc[0] = a * direction[0];
    acc[1] = a * direction[1];
}

GLFWwindow *StartGLFW()
{
    // Initialiser GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Erreur : impossible d'initialiser GLFW.\n");
        return NULL;
    }
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Gravity", NULL, NULL);

    glfwMakeContextCurrent(window);

    return window;
}

void draw_object(Object *obj)
{
    double rad = obj->radius;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(obj->x, obj->y);
    for (int i = 0; i <= RES; i++)
    {
        double angle = 2. * PI * ((double)i / RES);
        double x = obj->x + (cos(angle) * rad);
        double y = obj->y + (sin(angle) * rad);
        glVertex2d(x, y);
    }
    glEnd();
}

void update_camera(GLFWwindow *window, double *zoom, double *camX, double *camY)
{
    int values[4];
    glGetIntegerv(GL_VIEWPORT, values);
    double ratioX = values[2] * 0.01 * (*zoom);
    double ratioY = values[3] * 0.01 * (*zoom);

    // Transform
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        *camX += ratioX * CAMERAFAC;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        *camX -= ratioX * CAMERAFAC;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        *camY += ratioY * CAMERAFAC;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        *camY -= ratioY * CAMERAFAC;

    // Scale
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        *zoom -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        *zoom += 0.01;
    if (*zoom < 0.01)
        *zoom = 0.01;

    // Update
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(*camX - WIDTH * *zoom * CAMERAFAC, *camX + WIDTH * *zoom * CAMERAFAC,
            *camY - HEIGHT * *zoom * CAMERAFAC, *camY + HEIGHT * *zoom * CAMERAFAC,
            -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int is_colliding(Object obj1, Object obj2)
{
    if (distance(obj1, obj2) - obj1.radius - obj2.radius < 0)
    {
        return 1;
    }
    return 0;
}

void collide(Object *obj1, Object *obj2)
{
    Vector2D n = {(obj2->x - obj1->x) / distance(*obj1, *obj2), (obj2->y - obj1->y) / distance(*obj1, *obj2)};
    double vnobj1 = obj1->vx * n.x + obj1->vy * n.y;
    double vnobj2 = obj2->vx * n.x + obj2->vy * n.y;
    double dvobj1 = (RESTCOEFF * obj2->mass * (vnobj2 - vnobj1) + obj1->mass * vnobj1 + obj2->mass * vnobj2) / (obj1->mass + obj2->mass);

    obj1->vx = dvobj1 * n.x;
    obj1->vy = dvobj1 * n.y;
}

double orbital_speed(double mass1, double mass2, double x1, double x2)
{
    double velocity = sqrt(GRAVITY * (mass1 + mass2) / sqrt((x1 - x2) * (x1 - x2)));

    return velocity;
}

int main()
{

    GLFWwindow *window = StartGLFW();
    double zoom = 1;
    double camX = 57909050000;
    double camY = 0;

    if (!window)
    {
        fprintf(stderr, "What's happening");
        return 0;
    }

    Object Sun = {
        0, 0,
        0, 0,
        1.9885e30,
        696342000};

    Object Mercury = {
        57909050000, 0,
        0, orbital_speed(Sun.mass, 3.3011e23, Sun.x, 57909050000),
        3.3011e23,
        2439700};

    Object Earth = {1.496e11, 0, 0, orbital_speed(Sun.mass, 5.9836e24, Sun.x, 1.496e11), 5.9736e24, 6378000};

    Object Moon = {
        Earth.x + 384400000, 0,
        0, Earth.vy + orbital_speed(Earth.mass, 7.35e22, Earth.x, Earth.x + 384400000),
        7.35e22,
        1737400};
    Object points[] = {
        Sun,
        Mercury,
        Earth,
        Moon};
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < sizeof(points) / sizeof(Object); i++)
        {
            for (int j = 0; j < sizeof(points) / sizeof(Object); j++)
            {
                if (i != j)
                {
                    double acceleration[2];
                    calculate_acc(points[j], points[i], acceleration);
                    points[j].vx += acceleration[0] * DELTA_T;
                    points[j].vy += acceleration[1] * DELTA_T;
                    points[j].x += points[j].vx * DELTA_T;
                    points[j].y += points[j].vy * DELTA_T;
                    draw_object(&points[j]);
                    if (is_colliding(points[j], points[i]))
                    {
                        printf("collide");
                        collide(&points[j], &points[i]);
                    }
                }
            }
        }
        // printf("point1 : x : %.2f , y : %.2f \n", points[0].x, points[0].y);
        // printf("point2 : x : %.2f , y : %.2f , vitesse x : %.2f\n", points[1].x, points[1].y, points[1].vx);
        update_camera(window, &zoom, &camX, &camY);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
