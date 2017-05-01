#define __WIN32__
#include "Program1.h"

#include "GL_SDL_WindowManager.h"
#include <iostream>

#include "SDL_types.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <math.h>
#include <time.h>

#include <stdlib.h>

#include <vector>
using namespace std;

#include "frustum.h"

void render(int x, int y, int width, int height);
void SDL_pollEvents();

static bool done = false;
static bool mouseDown = false;
static bool mouseWasDown = false;

static Frustum frontFrustum;
static Frustum sideFrustum;
static Frustum topFrustum;
static Frustum backFrustum;

static int mouseX;
static int mouseY;

static int key = 0;

static vector<float> coords;
static vector<float> colors;

static float getNewColor(float oldColor) 
{
    if (oldColor < .05 || oldColor > .95) {
        return .5;
    }
    float newColor = oldColor;
    int ran = rand() % 30;
    if (ran < 10) {
        newColor = oldColor + .05;
    } else if (ran < 20) {
        newColor = oldColor - .05;
    }
    return newColor;
}

static void addColor() 
{

                int size = colors.size();
                if (size < 3) {
                    colors.push_back(.5);
                    colors.push_back(.5);
                    colors.push_back(.5);
                } else {
                    int index = size - 1;
                    colors.push_back(getNewColor(colors[index - 2]));
                    colors.push_back(getNewColor(colors[index - 1]));
                    colors.push_back(getNewColor(colors[index]));
                }
}

static void performActions();

void Program1::run()
{
    srand(time(0));
    GL_SDL_WindowManager win;
    if (!win.createWindow(1680, 1050)) {
        exit(1);
    }

    glMatrixMode(GL_PROJECTION);

    glFrustum(frontFrustum.getFrontUpperLeft()[0],
              frontFrustum.getFrontUpperRight()[0],
              frontFrustum.getFrontLowerLeft()[1],
              frontFrustum.getFrontUpperLeft()[1],
              - frontFrustum.getFrontUpperLeft()[2] - .01,
              - frontFrustum.getBackUpperLeft()[2]);


    while(!done) {
        SDL_pollEvents();
        
        performActions();

        glMatrixMode(GL_MODELVIEW);                      

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glMultMatrixf(&(frontFrustum.getInverse())[0]);
        render(0, 0, 1680, 1050);

        glFinish();
        glFlush();
        win.swapBuffers();
        //usleep(30000);
    }
}

 
void render(int vx, int vy, int width, int height)
{              
    glViewport(vx, vy, width, height);

    if (coords.size() >= 6) {
        glLineWidth(3);
        glBegin(GL_LINES);                                 
            for (int i = 0; i < coords.size(); i += 6) {
                glColor3f(colors[i], colors[i+1], colors[i+2]);
                glVertex3f(coords[i], coords[i+1], coords[i+2]);
                glColor3f(colors[i+3], colors[i+4], colors[i+5]);
                glVertex3f(coords[i+3], coords[i+4], coords[i+5]);
            }
        glEnd();                                           
    }
}                                              

void SDL_pollEvents()
{
    // Mouse location
    int x = 0;
    int y = 0;

    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch(sdlEvent.type) {

            case SDL_KEYDOWN:
                key = sdlEvent.key.keysym.sym;
                break;

            case SDL_KEYUP:
                key = 0;
                break;

            case SDL_MOUSEMOTION:
            {
                x = sdlEvent.button.x;
                y = sdlEvent.button.y;

                if (mouseDown) {
                    mouseX = x;
                    mouseY = y;
                }

            }
                break;

            case SDL_MOUSEBUTTONUP:
                mouseDown = false;
                mouseWasDown = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseDown = true;

                mouseX = sdlEvent.button.x;
                mouseY = sdlEvent.button.y;

                vector<float> worldPoint;
                worldPoint = frontFrustum.convertScreenToWorld(mouseX, mouseY, 1680, 1050);
                coords.push_back(worldPoint[0]);
                coords.push_back(worldPoint[1]);
                coords.push_back(worldPoint[2]);
                addColor();
 

                break;
        }
    }
}

void performActions() 
{
    if (key == SDLK_ESCAPE) {
        // Handle escape event
        done = true;
    }
    if (key == SDLK_F2) {
        // Handle F2 key press
    }
    if (key == 'a') {
                    frontFrustum.rotateY(.03);
                    topFrustum.rotateY(.03);
                    backFrustum.rotateY(.03);
    }
    if (key == 'd') {
                    frontFrustum.rotateY(-.03);
                    topFrustum.rotateY(-.03);
                    backFrustum.rotateY(-.03);
    }
    if (key == 'w') {
                    frontFrustum.moveForward(-.3);
                    topFrustum.moveForward(-.3);
                    backFrustum.moveForward(-.3);
    }
    if (key == 's') {
                   frontFrustum.moveForward(.3);
                    topFrustum.moveForward(.3);
                    backFrustum.moveForward(.3);
    } 
                if (mouseDown) {
                    vector<float> worldPoint;
                    worldPoint = frontFrustum.convertScreenToWorld(mouseX, mouseY, 1680, 1050);

                    if (mouseWasDown && coords.size() >= 3) {
                         int index = coords.size() - 1;
                         float previousZ = coords[index--];
                         float previousY = coords[index--];
                         float previousX = coords[index--];
                         coords.push_back(previousX);
                         coords.push_back(previousY);
                         coords.push_back(previousZ);
                         addColor();
                    }
                    mouseWasDown = true;
                    coords.push_back(worldPoint[0]);
                    coords.push_back(worldPoint[1]);
                    coords.push_back(worldPoint[2]);
                    addColor();
                     
                }
}
