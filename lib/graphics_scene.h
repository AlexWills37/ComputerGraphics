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
    // Member variables
    public:
        float scale[3];
        float rotation[3];
        float translation[3];

    // Constructors
    public:
        /*
        * Default Constructor. Creates a Transform at (0, 0, 0) with a scale of 1 and no rotation (facing positive Z).
        */
        Transform() : Transform(1, 1, 1, 0, 0, 0, 0, 0, 0)
        {}

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
         * Copy constructor. Explicitly copies array data.
         */
        Transform(const Transform& to_copy)
        {
            scale[0] = to_copy.scale[0];
            scale[1] = to_copy.scale[1];
            scale[2] = to_copy.scale[2];
            rotation[0] = to_copy.rotation[0];
            rotation[1] = to_copy.rotation[1];
            rotation[2] = to_copy.rotation[2];
            translation[0] = to_copy.translation[0];
            translation[1] = to_copy.translation[1];
            translation[2] = to_copy.translation[2];
        }

    // Operators
    public:
        /*
        * Casts a Transform object to its equivalent 4x4 TransformMatrix.
        */
        operator TransformMatrix() const;


    // Methods
    public: 
        /*
        * Move in local space, where movement in the positive Z direction is movement
        * in the direction of this transform's rotation.
        */
        void MoveLocally(float deltaX, float deltaY, float deltaZ);

        // void RotateAboutAxis(HomCoordinates axis_vector, float rotation);
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
    // Member variables
    protected:

        struct Model * model;
        Transform transform;

    // Constructors
    public:
        /*
         * Default constructor. Does not point to a model.
         */
        ModelInstance()
        {
            this->model = nullptr;
            this->transform = Transform();
        }
        /*
         * Constructs a model instance with a model pointer and a transform.
         */
        ModelInstance(Model * model, Transform & transform)
        {
            this->model = model;
            this->transform = transform;
        }

        /*
         * Copy constructor. Points to the same Model, but creates a copy of the transform.
         */
        ModelInstance(const ModelInstance & to_copy)
        {
            this->model = to_copy.model;
            this->transform = Transform(to_copy.transform); // Creates a copy of the transform
        }

        // ~ModelInstance();

    // Methods
    public:
        /*
         * Gets the pointer to this instance's model.
         */
        Model * GetModel()
        {
            return this->model;
        }


};

/*
 * RenderableModelInstance is a heavier-weight version of the ModelInstance
 * that calculates and stores the points/triangles to be used in the render pipeline.
 * This object should be destroyed as soon as it is no longer in use.
 * 
 * .GenerateWorldspacePoints() - apply transform to the model
 * .ApplyTransform( camera_matrix ) - move points to camera space
 * .GenerateBoundingSphere() - generate the bounding sphere
 * 
 * Check if bounding sphere is in bounds, or reject the model with .Reject()
 * Clip against plane if there is an intersection with .ClipTrianglesAgainstPlane( Plane* )
 */
class RenderableModelInstance: public ModelInstance {
    friend class Scene;
    
    // Member variables
    private:
        std::vector<HomCoordinates> points; // List of 3D points (worldspace, modelspace, or cameraspace)
        std::vector<Triangle> triangles;    // List of triangles for rendering
        bool is_rejected;   // True if the instance should not be rendered (all points are outside of a plane)
        HomCoordinates bounding_sphere_center;  // The center of a sphere including all points
        float bounding_sphere_radius;           // The radius of a sphere including all points

        // These following variables are used as buffers for clipping triangles
        
        std::vector<Triangle> new_tris; // List of new triangles to add to this render instance
        std::vector<HomCoordinates> new_points; // List of new points to add to this render instance, for the new triangles
        int new_point_start_index; // Index to start building new triangles

        // Debugging boolean
        bool in_camera_space = false;
    
    // Constructors
    public:

        /*
         * Default constructor. This instance should not be rendered.
         */
        RenderableModelInstance(): ModelInstance()
        {
            this->is_rejected = true;
            this->points.clear();
            this->triangles.clear();
            this->bounding_sphere_center = HomCoordinates();
            this->bounding_sphere_radius = 0;
            this->new_tris.clear();
            this->new_points.clear();
            this->new_point_start_index = 0;
        }

        /*
         * Constructor for a RenderableModelInstance using a ModelInstance.
         * This instance has a list of model triangles, but no points.
         */
        RenderableModelInstance(ModelInstance * instance): ModelInstance(*instance)
        {
            // Set this instance to be rendered with its list of triangles
            this->is_rejected = false;  
            this->triangles = this->model->triangles;
            this->new_point_start_index = this->triangles.size();


            this->points.clear();
            this->bounding_sphere_center = HomCoordinates();
            this->bounding_sphere_radius = 0;
            this->new_tris.clear();
            this->new_points.clear();       
        }

        /*
         * Copy constructor. Explicitly copies array data.
         */
        RenderableModelInstance(const RenderableModelInstance& to_copy)
        {
            this->is_rejected = to_copy.is_rejected;

            // Explicitly copy vectors
            // Points vector
            this->points.resize(to_copy.points.size());
            for (int i = 0; i < to_copy.points.size(); ++i)
            {
                this->points[i] = to_copy.points[i];
            }

            // Triangles vector
            this->triangles.resize(to_copy.triangles.size());
            for (int i = 0; i < to_copy.triangles.size(); ++i)
            {
                this->triangles[i] = to_copy.triangles[i];
            }
            this->bounding_sphere_center = to_copy.bounding_sphere_center;
            this->bounding_sphere_radius = to_copy.bounding_sphere_radius;

            // Copy buffer vectors
            this->new_tris.resize(to_copy.new_tris.size());
            for (int i = 0; i < to_copy.new_tris.size(); ++i)
            {
                this->new_tris[i] = to_copy.new_tris[i];
            }

            this->new_points.resize(to_copy.new_points.size());
            for (int i = 0; i < to_copy.new_points.size(); ++i)
            {
                this->new_points[i] = to_copy.new_points[i];
            }
            this->new_point_start_index = to_copy.new_point_start_index;


            this->in_camera_space = to_copy.in_camera_space;
        }

        // Destructor
        // ~RenderableModelInstance();

    // Methods
    public:
        /*
         * Uses the model pointer and transform to create a list of points in worldspace.
         * This overwrites the points list to use the original model (undoes clipping).
         */
        void GenerateWorldspacePoints();

        /*
         * Applies a TransformMatrix to every point in this model's list.
         */
        void ApplyTransform(TransformMatrix transform);

        /*
         * Returns a pointer to this instance's list of coordinates.
         */
        std::vector<HomCoordinates> * GetPoints()
        {
            return &(this->points);
        }

        /*
         * Returns a pointer to this instance's list of triangles.
         */
        std::vector<Triangle> * GetTriangles()
        {
            return &(this->triangles);
        }

        /*
         * Based on the current list of points, generate a bounding sphere to contain the model's points.
         */
        void GenerateBoundingSphere();

        /*
         * Sets this model's rejected value to true.
         */
        void Reject()
        {
            this->is_rejected = true;
        }

        /*
         * Clips this instance's points and triangles against a plane.
         */
        void ClipTrianglesAgainstPlane(Plane * plane);

        // Getters
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

    private:
        /*
         * Clips a triangle against a plane.
         */
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
        float viewport_distance, viewport_width, viewport_height;   // Dimensions of the viewport, which is centered in front of the camera's transform
        float canvas_width, canvas_height;  // Dimensions of the canvas, which is used to project points to the screen
        std::array<Plane, 5> clipping_planes;   // The planes used for clipping objects

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
         * Default constructor. Creates a camera at (0, 0, 0) facing the positive Z direction.
         * This constructor should never be called intentionally, since it sets the canvas
         * and viewport dimensions to 1x1.
         */
        Camera(): Camera(1, 1, 1.0, 1.0, 1.0)
        {
            this->camera_transform = Transform();   // The scale will always be 1
        }

        // Copy constructor
        // Camera(const Camera& to_copy)

        // Destructor
        // ~Camera()

    public: // Public methods

        /*
         * Returns a pointer to one of this camera's clipping planes.
         * @param index - the index [0, 4] of the clipping plane to access
         * @return (Plane*) a pointer to the plane
         */
        Plane * GetClippingPlane(int index)
        {
            if (index < 0 || index >= 5)
            {
                std::cout << "ERROR: Index out of bounds for clipping plane " << index << ". Returning a pointer to Plane [0] isntead." << std::endl;
                return &(this->clipping_planes[0]);
            }
            return &(this->clipping_planes[index]);
        }

        /*
         * Changes the camera's viewport distance and regenerates the clippng planes accordingly.
         * @param new_dist (float) - the distance (in world space units) away from the origin the clipping plane should be
         */
        void SetViewportDistance(float new_dist)
        {
            this->viewport_distance = new_dist;
            this->GenerateClippingPlanes();
        }

        /*
         * Changes the camera's viewport dimensions and regenerates the clipping planes accordingly.
         * @param width, height (float) - the dimensions (in world space units) of the camera's viewport
         */
        void SetViewportSize(float width, float height)
        {
            this->viewport_width = width;
            this->viewport_width = height;
            this->GenerateClippingPlanes();
        }

        /*
         * Returns a pointer to this camera's modifiable transform.
         */
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
        /*
         * Using the viewport dimensions, regenerates the clipping planes.
         */
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
        std::vector<ModelInstance*> model_instances;  // List of models to render

        Camera* main_camera;    // Camera to render models from
        GraphicsManager* graphics_manager;  // GraphicsManager to perform draw calls
    
    // Constructors
    public:
        /*
         * Constructs a scene with a camera and a graphics manager.
         * 
         * @param main_camera - pointer to the camera to view the scene from
         * @param graphics_manager - pointer to the GraphicsManager that draws pixels to the screen
         */
        Scene(Camera* main_camera, GraphicsManager* graphics_manager)
        {
            this->main_camera = main_camera;
            this->graphics_manager = graphics_manager;
        }

        /*
         * Constructs an empty scene without a camera or a GraphicsManager.
         * This Scene cannot be rendered. To create a render-able scene,
         * pass in a pointer to the camera and GraphicsManager to this constructor.
         */
        Scene() : Scene(nullptr, nullptr)
        {}

        /*
         * Copy constructor. Explicitly copies every model instance as a pointer to a COPY
         * of the ModelInstance (as opposed to a copy of the pointer, pointing to the same instances).
         */
        Scene(const Scene& to_copy);

        // Destructor
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
         * Render an individual RenderableModelInstance to the screen
         * based on its current list of points and triangles.
         */
        void RenderInstance(RenderableModelInstance * to_render);

        /*
         * Based on a list of projected vertices, render an individual triangle to the screen.
         */
        void RenderTriangle(Triangle triangle, Point2D projected_vertices[]);

    // Static helper methods
    private:
        /*
         * Given 5 clipping planes, clip a model instance so that 
         * we do not draw points outside of the viewport.
         */
        static void ClipInstance(RenderableModelInstance & instance, std::array<Plane*, 5> planes);

        /*
         * Clip an isntance against a sigle plane, so that none of its points are outside the plane.
         */
        static void ClipInstanceAgainstPlane(RenderableModelInstance & instance, Plane* plane);       
};

#endif