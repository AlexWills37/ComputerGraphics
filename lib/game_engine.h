

#include "graphics.h"


class Behavior
{
    public:
        Behavior()
        {

        }
    
    public:
        void Update();
};


class GameEngine
{
    private:
        GraphicsManager graphics;
        InputModule input_module;

    public:
        GameEngine()
        {

        }

        ~GameEngine()
        {

        }
        
    public:
        void AddBehavior()

};