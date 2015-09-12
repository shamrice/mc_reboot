#include "Engine.h"
#include "Timer.h"
#include "Enums.H"

#define SCREEN_HEIGHT 1024
#define SCREEN_WIDTH 768
#define FRAMES_PER_SECOND 40

//globals
const bool FULLSCREEN = false;
Engine *mcEngine;

//function prototypes
void ProcessInput(bool&, bool&);
void HandleEvents();

int main ( int argc, char** argv )
{
    bool restart = false;
    bool quit = false;
    mcEngine = new  Engine(SCREEN_HEIGHT, SCREEN_WIDTH, FULLSCREEN);
    Timer fpsTimer;

    if (!mcEngine->EngineError)
    {

        while (!quit)
        {
            while (!restart)
            {
                //start frame rate timer
                fpsTimer.Start();

                ProcessInput(restart, quit);

                mcEngine->HandleEvents();

                // DRAWING STARTS HERE
                //clear screen
                mcEngine->ClearScreen();

                mcEngine->RenderBackground();
                // draw bitmap
                mcEngine->RenderSprites();

                //update the screen :)
                mcEngine->UpdateScreen();

                // DRAWING ENDS HERE

                //regulate FPS
                if(fpsTimer.GetTicks() < 1000 / FRAMES_PER_SECOND)
                {
                    SDL_Delay((1000 / FRAMES_PER_SECOND) - fpsTimer.GetTicks());
                }

            } // end main loop

            //if not quiting application, restart the engine.
            if (!quit) {
                mcEngine->Restart();
                restart = false;
            }
        }
        printf("Exited cleanly\n");
    }
    else
    {
        printf("Game Engine has encountered an unrecoverable error.\nApplication terminated.\n");
        return -1;
    }

    return 0;
}

void ProcessInput(bool& restart, bool& quit)
{
    //printf("process input ");
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            restart = true;
            quit = true;
            break;

            // check for keypresses
        case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                restart = true;
                quit = true;
            }
            else if (event.key.keysym.sym == SDLK_SPACE)
                mcEngine->FireProjectile();
            else if (event.key.keysym.sym == SDLK_e)
                mcEngine->ResetBadGuy();
            else if (event.key.keysym.sym == SDLK_r)
                restart = true;
            else if (event.key.keysym.sym == SDLK_i)
                mcEngine->PrintPlayerInventory();
            else if (event.key.keysym.sym == SDLK_h)
                mcEngine->UseItem(HEALTH_ITEM);
            else if (event.key.keysym.sym == SDLK_w)
                mcEngine->UseItem(WEAPON);
            else if (event.key.keysym.sym == SDLK_k)
                mcEngine->UseItem(KEY);

            break;
        }
        } // end switch
    } // end of message processing

    //check for continous key press
    Uint8* keystate = SDL_GetKeyState(NULL);

    if (keystate[SDLK_UP])
        mcEngine->MovePlayer(UP);
    else if (keystate[SDLK_DOWN])
        mcEngine->MovePlayer(DOWN);
    else if (keystate[SDLK_LEFT])
        mcEngine->MovePlayer(LEFT);
    else if (keystate[SDLK_RIGHT])
        mcEngine->MovePlayer(RIGHT);

}



