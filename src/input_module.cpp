#include "../lib/input_module.h"

Input InputModule::GetBinding(SDL_Keycode keycode)
{
    Input binding = Input::NONE;
    switch (keycode)
    {
        case SDLK_SPACE:
            binding = Input::SPACE;
            break;

        case SDLK_ESCAPE:
            binding = Input::QUIT;
            break;
        
        case SDLK_w:
            binding = Input::W;
            break;
        case SDLK_a:
            binding = Input::A;
            break;
        case SDLK_s:
            binding = Input::S;
            break;
        case SDLK_d:
            binding = Input::D;
            break;
        
        case SDLK_e:
            binding = Input::E;
            break;
        
        case SDLK_q:
            binding = Input::Q;
            break;
        
        case SDLK_i:
            binding = Input::I;
            break;
        case SDLK_j:
            binding = Input::J;
            break;
        case SDLK_k:
            binding = Input::K;
            break;
        case SDLK_l:
            binding = Input::L;
            break;
        
        case SDLK_UP:
            binding = Input::UP;
            break;
        case SDLK_DOWN:
            binding = Input::DOWN;
            break;
        case SDLK_LEFT:
            binding = Input::LEFT;
            break;
        case SDLK_RIGHT:
            binding = Input::RIGHT;
            break;

        case SDLK_LSHIFT:
            binding = Input::SHIFT;
            break;

        case SDLK_LCTRL:
            binding = Input::CTRL;
            break;  
        default:
            binding = Input::NONE;
            break;
    }

    return binding;
}

void InputModule::UpdateInputs()
{
    // Handle events in our map
    SDL_Keycode key;
    Input binding;
    while(SDL_PollEvent(&(this->event_handler)))
    {
        switch (this->event_handler.type)
        {
            case SDL_QUIT:
                this->input_states[Input::QUIT] = true;
                break;
            
            // For key down OR key up, we must figure our the key binding
            case SDL_KEYDOWN:
                // Get key binding
                key = event_handler.key.keysym.sym; // This is the scancode of the key being pressed
                binding = InputModule::GetBinding(key);

                // Update the key value (keydown -> key_state == true)
                if (binding != Input::NONE)
                {
                    this->input_states[binding] = true;
                }
                
                break;

            case SDL_KEYUP:
                // Get key binding
                key = event_handler.key.keysym.sym; // This is the scancode of the key being pressed
                binding = InputModule::GetBinding(key);

                // Update the key value (keyup -> key_state == false)
                if (binding != Input::NONE)
                {
                    this->input_states[binding] = false;
                }
                
                break;

            default:
                break;
        }
    }

    // Update any listeners that observe a new event


}
