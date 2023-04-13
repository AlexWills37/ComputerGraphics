#include "../lib/graphics.h"
#include "../lib/input_module.h"
#include <ctime>
#include <array>

HomCoordinates MovementVector(float rotation[3], float localDirection[3])
{
    HomCoordinates vector;
    vector.data[0] = localDirection[0];
    vector.data[1] = localDirection[1];
    vector.data[2] = localDirection[2];
    vector.data[3] = 0;
    
    vector = BuildRotationMatrix(rotation[0], rotation[1], rotation[2]) * vector;
    return vector;
}


/*
 * Keeps the user's window open and allows the user to move the camera in the scene.
 * Use WASD to move on the X/Z plane, E and Q to move up/down on the Y axis,
 * and the arrow keys or IJKL to rotate up/down and left/right.
 */
void GraphicsManager::StayOpenCameraControls()
{
	InputModule input = InputModule(this->event_handler);

	
	bool running = true;

	float delta_time;
	clock_t previous_clock = std::clock();
	clock_t current_clock;
	float fps = 120;

	float time_between_frames = 1.0 / fps;

	Transform * camera = this->GetMainCamera()->GetTransform();
	Camera * real_camera = this->GetMainCamera();
	
	std::array<float, 3> velocity;

	float speed = 5;
	float modifier = 1;
	float rotation_speed = 0.02;

	// int * test = new int();

	while (running)
	{

		// Update inputs
		input.UpdateInputs();

		// Update time to see if it is time for a new frame
		current_clock = std::clock();
		delta_time = float(current_clock - previous_clock) / (CLOCKS_PER_SEC);

		if (delta_time >= time_between_frames)
		{
			// delete test;
			// test = new int();
			// std::cout << test << std::endl;
			previous_clock = current_clock;
			// Frame is starting; do frame-based things



			// Act according to inputs
			if (input.IsInputPressed(Input::QUIT))
			{
				running = false;
				std::cout << "~ User has closed the window +" << std::endl;
			}

			// Move camera
			velocity[0] = 0;
			velocity[1] = 0;
			velocity[2] = 0;
			if (input.IsInputPressed(Input::W))	// Forward
			{
				velocity[2] += 1;
			}
			if (input.IsInputPressed(Input::A))	// Left
			{
				velocity[0] += -1;
			}
			if (input.IsInputPressed(Input::S))	// Back
			{
				velocity[2] += -1;
			}
			if (input.IsInputPressed(Input::D))	// Right
			{
				velocity[0] += 1;
			}
			if (input.IsInputPressed(Input::E))	// Up
			{
				velocity[1] += 1;
			}
			if (input.IsInputPressed(Input::Q)) // Down
			{
				velocity[1] += -1;
			}
			
			if (input.IsInputPressed(Input::SHIFT))
			{
				modifier = 3;
			} else if (input.IsInputPressed(Input::CTRL))
			{
				modifier = 0.5;
			} else 
			{
				modifier = 1;
			}

			NormalizeVector(velocity);
			camera->MoveLocally(velocity[0] * modifier * speed * delta_time, 
				velocity[1] * modifier * speed * delta_time, velocity[2] * modifier * speed * delta_time);
			
			// Rotate camera
			// if (input.IsInputPressed(Input::LEFT))
			// {
			// 	real_camera->yaw += rotation_speed;
			// }
			// if (input.IsInputPressed(Input::RIGHT))
			// {
			// 	real_camera->yaw -= rotation_speed;
			// }
			// if (input.IsInputPressed(Input::UP))
			// {
			// 	real_camera->pitch += rotation_speed;
			// }
			// if (input.IsInputPressed(Input::DOWN))
			// {
			// 	real_camera->pitch -= rotation_speed;
			// }

			// Rotate camera
			if (input.IsInputPressed(Input::LEFT))
			{
				camera->rotation[1] += rotation_speed;
			}
			if (input.IsInputPressed(Input::RIGHT))
			{
				camera->rotation[1] -= rotation_speed;
			}
			if (input.IsInputPressed(Input::UP))
			{
				camera->rotation[0] += rotation_speed;
			}
			if (input.IsInputPressed(Input::DOWN))
			{
				camera->rotation[0] -= rotation_speed;
			}

			if (input.IsInputPressed(Input::SPACE))
			{
				camera->translation[0] = 0;
				camera->translation[1] = 0;
				camera->translation[2] = 0;
				camera->rotation[2] = 0;
				camera->rotation[1] = 0;
				camera->rotation[0] = 0;
			}
		

			// Update screen and render new scene
			this->ChangeBrushColor(BLACK);
			this->ClearScreen();
			this->current_scene.RenderScene();
			this->RefreshScreen();
		}

	}
	
	// bool running = true;
	// SDL_Keysym key;
    // float localVelocity[3];
    // bool move = false;
	// while(running)
	// {
    //     move = false;
	// 	// Go through the event queue
	// 	while ( SDL_PollEvent(&event_handler) )
	// 	{
	// 		// Handle the event
	// 		switch (event_handler.type)
	// 		{	
	// 			// Quit event
	// 			case SDL_QUIT:
	// 				running = false;
	// 				std::cout << "~ User has closed the window." << std::endl;
	// 				break;
	// 			case SDL_KEYDOWN:
	// 				key = event_handler.key.keysym;
    //                 localVelocity[0] = 0;
    //                 localVelocity[1] = 0;
    //                 localVelocity[2] = 0;


	// 				//std::cout << "~ User has pressed key: " << key.sym << std::endl;
	// 				switch (key.sym)
	// 				{
	// 					case SDLK_ESCAPE:
	// 						std::cout << "~ User pressed [esc] - closing window." << std::endl;
	// 						running = false;
	// 						break;
	// 					// Move left
	// 					case SDLK_a:
	// 						// this->main_camera.MovePosition(-0.1, 0, 0);
    //                         localVelocity[0] = -0.1;
    //                         localVelocity[1] = 0;
    //                         localVelocity[2] = 0;
    //                         move = true;
	// 						break;
	// 					// Move back
	// 					case SDLK_s:
	// 						// this->main_camera.MovePosition(0, 0, -0.1);
    //                         localVelocity[0] = 0;
    //                         localVelocity[1] = 0;
    //                         localVelocity[2] = -0.1;
    //                         move = true;
	// 						break;
	// 					// Move right
	// 					case SDLK_d:
	// 						// this->main_camera.MovePosition(0.1, 0, 0);
    //                         localVelocity[0] = 0.1;
    //                         localVelocity[1] = 0;
    //                         localVelocity[2] = 0;
    //                         move = true;
	// 						break;
	// 					// Move forward
	// 					case SDLK_w:
	// 						// this->main_camera.MovePosition(0, 0, 0.1);
    //                         localVelocity[0] = 0;
    //                         localVelocity[1] = 0;
    //                         localVelocity[2] = 0.1;
    //                         move = true;
	// 						break;
	// 					// Move down
	// 					case SDLK_q:
	// 						// this->main_camera.MovePosition(0, -0.1, 0);
    //                         localVelocity[0] = 0;
    //                         localVelocity[1] = -0.1;
    //                         localVelocity[2] = 0;
    //                         move = true;
	// 						break;

	// 					// Move up
	// 					case SDLK_e:
	// 						// this->main_camera.MovePosition(0, 0.1, 0);
    //                         localVelocity[0] = 0;
    //                         localVelocity[1] = 0.1;
    //                         localVelocity[2] = 0;
    //                         move = true;
	// 						break;

	// 					// Rotate up
	// 					case SDLK_i:
	// 						// Fall through
	// 					case SDLK_UP:
	// 						this->main_camera.Rotate(3.14/60, 0, 0);
	// 						break;

	// 					// Rotate down
	// 					case SDLK_k:
	// 						// Fall through
	// 					case SDLK_DOWN:
	// 						this->main_camera.Rotate(-3.14/60, 0, 0);
	// 						break;
	// 					// Rotate counter-clockwise
	// 					case SDLK_j:
	// 						// Fall through
	// 					case SDLK_LEFT:
	// 						this->main_camera.Rotate(0, 3.14/60, 0);
	// 						break;
						
	// 					// Rotate clockwise
	// 					case SDLK_l:
	// 						// Fall through
	// 					case SDLK_RIGHT:
	// 						this->main_camera.Rotate(0, -3.14/60, 0);
	// 						break;

	// 					case SDLK_SPACE:
	// 						this->rotate_cube->transform.scale[0] += 0.5;
	// 						this->rotate_cube->transform.scale[1] += 0.5;
	// 						this->rotate_cube->transform.scale[2] += 0.5;
	// 						break;

	// 					default:
	// 						// Do nothing if key not recognized
	// 						break;
	// 				}

					

	// 				break;
	// 			default:
	// 				break;
	// 		}
	// 	}

    //     // Move camera
    //     if (move)
    //     {
    //         HomCoordinates globalVelocity = MovementVector(this->main_camera.GetRotation(), localVelocity);
    //         this->main_camera.MovePosition(globalVelocity.data[0], globalVelocity.data[1], globalVelocity.data[2]);
    //         // this->main_camera.MovePosition(localVelocity[0], localVelocity[1], localVelocity[2]);
    //     }

	// 	this->RotateCube(this->rotate_cube);

	// 	// Clear screen
	// 	this->ChangeBrushColor(BLACK);
	// 	this->ClearScreen();
	// 	// Render new scene
	// 	this->current_scene.RenderScene();
	// 	this->RefreshScreen();

	//}
}

