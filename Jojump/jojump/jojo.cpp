#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

struct point { int x,y; };

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(400, 533), "JoJo Jump!");
    app.setFramerateLimit(120);
    // Textures
    Texture t1,t2,t3;
    t1.loadFromFile("images/background_sky.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/Jotaro_angry_final.png");

    // Sprites
    Sprite sBackground(t1), sPlat(t2), sPers(t3);

    // Tavtsanguud Random
    point plat[6];								// <- HERE!!!
    for (int i=0;i<3;i++)               					// <- here bro
    {
        plat[i].x=rand()%333;
        plat[i].y=i*177;
    }

    // Character
    int x=plat[2].x,y=100,h=200;
    float dx=0,dy=0;

    // Background
    float bgY = 0;
    // Window haah
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        // Controls
        if (Keyboard::isKeyPressed(Keyboard::Right)) x+=8;
        if (Keyboard::isKeyPressed(Keyboard::Left))  x-=8;

        // Tatah huch
        dy+=0.2;
        y+=dy;

        // Doosh unaval usreh
        if (y>500){
            y=100;
            dy=0;
        }

        // Tavtsang tanih
        for (int i=0;i<3;i++)   							// <-here bro
        {
            if ((x+50 > plat[i].x) && (x+20 < plat[i].x+68)  // tavtsangiin x tenhleg
                && (y+70 > plat[i].y-10) && (y+70 < plat[i].y+14) // Tavtsangiin deer hureh
                && (dy > 0)) // Unaj baih
            {
                dy = -10; // Deesh usreh
            }
        }

        // Camera system
        if (y<h)
        {
            y=h;
            for (int i=0;i<3;i++)							//<- here bro
            {
                plat[i].y = plat[i].y - dy;

                if (plat[i].y>533)
                {
                    plat[i].y=0;
                    plat[i].x=rand()%333;
                }
            }

            // Ariin tengeriig udaan hodolgoh
            bgY -= dy*0.3;
        }

        // Draw
        app.clear();

        // Ariin zurag (infinite)
        sBackground.setPosition(0,bgY);
        app.draw(sBackground);
        sBackground.setPosition(0, bgY-532);
        app.draw(sBackground);

        // Reset background pos
        if (bgY > 533) bgY = 0;
        if (bgY < 0)   bgY += 533;

        // Tavtsanguudiig zurah
        for (int i=0;i<3;i++)								// <- here bro
        {
            sPlat.setPosition(plat[i].x,plat[i].y);
            app.draw(sPlat);
        }

        // Jojog zurah
        sPers.setPosition(x,y);
        app.draw(sPers);

        app.display();
    }

    return 0;
}
