#include "../lib/graphics.h"
#include "../lib/input_module.h"
#include <ctime>
#include <array>

HomCoordinates MovementVector(float rotation[3], float localDirection[3])
{
    HomCoordinates vector;
    vector[0] = localDirection[0];
    vector[1] = localDirection[1];
    vector[2] = localDirection[2];
    vector[3] = 0;
    
    vector = TransformMatrix::BuildRotationMatrix(rotation[0], rotation[1], rotation[2]) * vector;
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
			// std::cout << "Pixel draw calls this frame: " << this->drawCount << std::endl;
			this->drawCount = 0;

		}

	}
}

