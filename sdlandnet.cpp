#include <iostream>
#include "sdlandnet.hpp"

int main(int argc, char** argv) {
    System::initialise(System::VIDEO);
    
    {
        Display display("SDL and Net", 500, 500);
        display.fill(Sprite::BLACK);
        display.update();
        
        Event event;
        
        bool quit = false;
        int lum = 0;
        
        while (!quit) {
            while (event.poll()) {
                if (event.type() == Event::TERMINATE) {
                    quit = true;
                }
                
                else if (event.type() == Event::LEFT_CLICK) {
                    lum = 255;
                    display.fill(lum);
                    display.update();
                }
                
                else if (event.type() == Event::MIDDLE_CLICK) {
                    lum = 127;
                    display.fill(lum);
                    display.update();
                }
                
                else if (event.type() == Event::RIGHT_CLICK) {
                    lum = 0;
                    display.fill(lum);
                    display.update();
                }
                
                else if (event.type() == Event::KEY_PRESS && event.key() == Events::ESCAPE) {
                    quit = true;
                }
                
                else if (event.type() == Event::SCROLL) {
                    lum += event.scroll().get_y();
                    display.fill(lum);
                    display.update();
                }
            }
        }
    }
    
    System::terminate();
    
    return 0;
}