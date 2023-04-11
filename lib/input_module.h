#ifndef _INPUT_MODULE_H
#define _INPUT_MODULE_H


#include <map>
#include <vector>
#include <SDL2/SDL.h>

enum class Input {
    NONE, QUIT, ESC, SPACE,
    W, A, S, D,
    LEFT, RIGHT, UP, DOWN,
    E, Q, I, J, K, L,
    SHIFT, CTRL
    
};

class InputListener {
    private:
        Input trigger;
    public:
        void OnEvent();
};  

class InputModule {
    private:
        SDL_Event event_handler;

        std::map<Input, bool> input_states;
        std::vector<InputListener> input_listeners;

    // Constructor
    public:
        InputModule(SDL_Event event_handler)
        {
            this->event_handler = event_handler;
            // InputModule::singleton = this;
        }
    
    // Static functions
    public:
        bool IsInputPressed(Input query)
        {
            return this->input_states[query];
        }

    
    // Public member functions
    public:
        void AddInputListener(InputListener to_add)
        {
            this->input_listeners.push_back(to_add);
        }
        void UpdateInputs();
         
    private:


        Input GetBinding(SDL_Keycode keycode); 

    

};


#endif