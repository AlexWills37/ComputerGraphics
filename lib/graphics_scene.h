/* graphics_scene.h
 *
 * This file contains the classes and methods for building 3D scenes to render.
 * A Scene contains a Camera and a list of ModelInstances.
 * A Camera holds a Transform specifying the position/rotation to view the scene from.
 * A Transform specifies the scale, rotation, and translation of an object.
 * A ModelInstance contains a Model to reference and a Transform to specify its location in world space.
 * A Model is a list of points and triangles (which connect points and have a color), where the points
 *      exist in model space (a relative origin)
 * 
 * @author Alex Wills
 * @date March 28, 2023
 */
#ifndef _GRAPHICS_SCENE_H
#define _GRAPHICS_SCENE_H
#include <vector>


// ------- Representing a scene with models and transforms --------
/*
 * Transform class
 * A Transform specifies the scale, rotation, and translation of an object with floats.
 * 
 * Transform objects can easily be type-cast to a 4x4 matrix ready for matrix multiplication, in
 * the form of a TransformMatrix.
 */
class Transform {
    public:
	float scale[3];
	float rotation[3];
	float translation[3];

    // Constructors
    /*
     * Creates a Transform with specified values.
     * @param sx, sy, sz - the scale values for x, y, and z
     * @param rx, ry, rz - the rotation values for x, y, and z
     * @param tx, ty, tz - the translation (position) values for x, y, and z
     */
    Transform(float sx, float sy, float sz, float rx, float ry, float rz, float tx, float ty, float tz)
    {
        scale[0] = sx;
        scale[1] = sy;
        scale[2] = sz;
        rotation[0] = rx;
        rotation[1] = ry;
        rotation[2] = rz;
        translation[0] = tx;
        translation[1] = ty;
        translation[2] = tz;
    }

    /*
     * Move in local space, where movement in the positive Z direction is movement
     * in the direction of this transform's rotation.
     */
    void MoveLocally(float deltaX, float deltaY, float deltaZ);

    void RotateAboutAxis(HomCoordinates axis_vector, float rotation);

    /*
     * Creates a Transform at (0, 0, 0) with a scale of 1 and no rotation (facing positive Z).
     */
    Transform() : Transform(1, 1, 1, 0, 0, 0, 0, 0, 0)
    {}

    /*
     * Casts a Transform object to its equivalent 4x4 TransformMatrix.
     */
    operator TransformMatrix() const;



};


/*
 * The Triangle struct links together 3 points (as indices) and assigns a color. 
 * Used for building models.
 */
struct Triangle {
	int p0, p1, p2;
	Color color;
};

/*
 * The Model struct contains a list of vertices (Points in Model Space)
 * and a list of triangles (containing the indices of the points to link together).
 */
struct Model {
    std::vector<Point3D> vertices;
    std::vector<Triangle> triangles;
};

/*
 * An instance of a model contains a pointer to a model to use,
 * along with a transform specifying the location of the model in World Space.
 */
class ModelInstance {
    
    public:
        ModelInstance()
        {}
        ModelInstance(Model * model, Transform & transform)
        {
            this->model = model;
            this->transform = transform;
        }

        // Default copy constructor
        // ModelInstance(const ModelInstance & to_copy)
        // {        }

        // ~ModelInstance();

    public:
        Model * GetModel()
        {
            return this->model;
        }

    protected:

        struct Model * model;
        Transform transform;

};

class RenderableModelInstance: public ModelInstance {
    friend class Scene;
    public:

        RenderableModelInstance(ModelInstance * instance): ModelInstance(*instance)
        {
            this->is_rejected = false;  
            this->triangles = this->model->triangles;
            // std::cout << "Creating Renderable Model Instance with " << triangles.size() << " triangles" << std::endl;
        }

        // RenderableModelInstance();
        // ~RenderableModelInstance();


    private:
        std::vector<HomCoordinates> points;
        std::vector<Triangle> triangles;
        bool is_rejected;
        HomCoordinates bounding_sphere_center;
        float bounding_sphere_radius;

        // These following variables are used as buffers for clipping triangles
        std::vector<Triangle> new_tris; // List of new triangles to add to this render instance
        std::vector<HomCoordinates> new_points; // List of new points to add to this render instance, for the new triangles
        int new_point_start_index = this->triangles.size(); // Index to start building new triangles

    public:
        void GenerateWorldspacePoints();

        void ApplyTransform(TransformMatrix transform);

        std::vector<HomCoordinates> * GetPoints()
        {
            return &(this->points);
        }

        void GenerateBoundingSphere();

        float GetBoundingSphereRadius()
        {
            return this->bounding_sphere_radius;
        }

        HomCoordinates GetBoundingSphereCenter()
        {
            return this->bounding_sphere_center;
        }

        bool GetIsRejected()
        {
            return this->is_rejected;
        }

        void Reject()
        {
            this->is_rejected = true;
        }

        void ClipTrianglesAgainstPlane(Plane * plane);

        bool ClipTriangle(Triangle to_clip, Plane * plane);
    
};

// -------- Rendering a Scene --------


/*
 * Camera class
 * The camera defines the location/orientation (among other settings in the future)
 * from where graphics will be rendered.
 * 
 * Typically, a scene will be viewed from this point in the forward direction of the camera.
 */
class Camera
{
    private: // Private member variables
        Transform camera_transform; // This is the transform of the camera
        float viewport_distance, viewport_width, viewport_height;
        float canvas_width, canvas_height;
        std::array<Plane, 5> clipping_planes;

    public: // Constructor
        
        /*
         * Creates a camera based on canvas dimensions and viewport dimensions. By default, the camera
         * will be placed at (0, 0, 0) facing the positive Z direction.
         */
        Camera(int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance)
        {
            this->canvas_width = canvas_width;
            this->canvas_height = canvas_height;
            this->viewport_width = viewport_width;
            this->viewport_height = viewport_height;
            this->viewport_distance = viewport_distance;

            this->camera_transform = Transform();
            this->GenerateClippingPlanes();
        }

        /*
         * Create a camera at (0, 0, 0) facing the positive Z direction.
         * This constructor should never be called intentionally, since it sets the canvas
         * and viewport dimensions to 1x1.
         */
        Camera(): Camera(1, 1, 1.0, 1.0, 1.0)
        {
            this->camera_transform = Transform();   // The scale will always be 1
        }

        /*
         * Create a camera at a specified location with a specified rotation 
         * (rotation starts from facing the positive Z direction).
         */
        Camera(float x, float y, float z, float rx, float ry, float rz) : Camera()
        {
            this->camera_transform.translation[0] = x;
            this->camera_transform.translation[1] = y;
            this->camera_transform.translation[2] = z;
            this->camera_transform.rotation[0] = rx;
            this->camera_transform.rotation[1] = ry;
            this->camera_transform.rotation[2] = rz;
        }

    public: // Public methods

        Plane * GetClippingPlane(int index)
        {
            return &(this->clipping_planes[index]);
        }

        void SetViewportDistance(float new_dist)
        {
            this->viewport_distance = new_dist;
            this->GenerateClippingPlanes();
        }

        void SetViewportSize(float width, float height)
        {
            this->viewport_width = width;
            this->viewport_width = height;
        }

        float GetViewportWidth()
        {
            return this->viewport_width;
        }

        float GetViewportHeight()
        {
            return this->viewport_height;
        }

        float GetViewportDistance()
        {
            return this->viewport_distance;
        }

        float* GetPosition()
        {
            return &(this->camera_transform.translation[0]);
        }

        float* GetRotation()
        {
            return &(this->camera_transform.rotation[0]);
        }

        Transform * GetTransform()
        {
            return &(this->camera_transform);
        }

        /*
        * Converts a point from 3D space to canvas space, based on this camera's
        * viewport. Assumes the camera is at (0, 0) facing towards the positive Z direction.
        *
        * @param vertex - the 3D point to render
        * @return the 3D point's location on the 2D canvas, as seen through the viewport
        */
        Point2D ProjectVertex(Point3D vertex);
        Point2D ProjectVertex(HomCoordinates vertex);


        /*
         * Sets the position of the camera in world space. Overwrites the previous position.
         * @param x, y, z - the location of the camera in world space.
         */
        void SetPosition(float x, float y, float z);

        /*
         * Shifts the position of the camera in world space based on the previous position.
         * @param deltaX, deltaY, deltaZ - the distance in x, y, and z (in world space) to move the camera
         */
        void MovePosition(float deltaX, float deltaY, float deltaZ);

        

        /*
         * Sets the rotation of the camera in world space. Overwrites the previous rotation.
         * @param x, y, z - the new rotation of the camera in world space, where (0, 0, 0) is the positive Z axis.
         */
        void SetRotation(float x, float y, float z);

        /*
         * Rotates the camera in the specified direction.
         * @param deltaX, deltaY, deltaZ - the amount about each axis to rotate the camera
         */
        void Rotate(float deltaX, float deltaY, float deltaZ);

        /*
         * Converts the camera's transform to a transformation matrix that can be applied
         * to coordinates in world space to convert them to Camera Space.
         * 
         * By multiplying this transform with World Space coordinates, you get new coordinates based
         * on the camera existing at (0, 0, 0) and facing the positive Z axis.
         */
        TransformMatrix GetWorldToCameraMatrix();

        /*
         * Rotates the camera on the X axis locally. Assumes that the x axis is parallel to the XZ plane.
         */
        void RotateVertically(float rotation);

    private:
        void GenerateClippingPlanes();
};

// Forward declare graphics manager
class GraphicsManager;

/*
 * Scene class
 * Holds models and a camera, and can be manipulated.
 */
class Scene
{
    // Private member variables
    private:
        std::vector<ModelInstance*> modelInstances;  // List of models to render

        Camera* main_camera;    // Camera to render models from
        GraphicsManager* graphics_manager;  // GraphicsManager to perform draw calls
    
    // Constructors
    public:
        /*
         * Construct a scene with a camera and a graphics manager.
         * 
         * @param main_camera - pointer to the camera to view the scene from
         * @param graphics_manager - pointer to the GraphicsManager that draws pixels to the screen
         */
        Scene(Camera* main_camera, GraphicsManager* graphics_manager)
        {
            this->main_camera = main_camera;
            this->graphics_manager = graphics_manager;
            this->is_original_scene = true;
        }

        /*
         * Construct an empty scene without a camera or a GraphicsManager.
         * This Scene cannot be rendered. To create a render-able scene,
         * pass in a pointer to the camera and GraphicsManager to this constructor.
         */
        Scene() : Scene(nullptr, nullptr)
        {}

        ~Scene();

    // Public methods
    public:
        /*
         * Add an instance of a model to the scene, to be rendered.
         * 
         * @param to_add (passed by reference) - the model to add to the scene.
         */
        void AddModelInstance(ModelInstance & to_add);

        /*
         * Renders the scene to the window created by the GraphicsManager.
         */
        void RenderScene();

    // Private helper methods
    private:
        /*
         * Render an individual ModelInstance to the screen.
         */
        void RenderInstance(RenderableModelInstance * to_render);

        /*
         * Based on a list of projected vertices, render an individual triangle to the screen.
         */
        void RenderTriangle(Triangle triangle, Point2D projected_vertices[]);


        static void ClipInstance(RenderableModelInstance & instance, std::array<Plane*, 5> planes);

        static void ClipInstanceAgainstPlane(RenderableModelInstance & instance, Plane* plane);

        bool is_original_scene;


        
};



#endif