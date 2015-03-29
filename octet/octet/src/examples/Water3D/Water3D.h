////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//


#include <chrono>
#include <ctime>
#include "WaterTerrain.h"

namespace octet {
  /// Scene containing a box with octet.
  class Water3D : public app {
    enum {_NUM_WAVES = 8};
    // scene for drawing box
    ref<visual_scene> app_scene;

    mouse_look mouse_look_helper;
    helper_fps_controller fps_helper;

    ref<scene_node> player_node;

    ref<camera_instance> the_camera;

    example_geometry_source source;
    water_geometry_source water_source;

    ref<material> water_material;
    ref<param_uniform> uniform_time;
    ref<param_uniform> uniform_number_waves;
    ref<mesh_instance> water_mesh_instance;

    WaveInfo waves_info;

    std::chrono::time_point<std::chrono::system_clock> strating;
    float time_per_frame;
    int number_waves;

    void set_up_water(const mat4t& mat){
      //Setting up parameters of the shader!
      param_shader* water_shader = new param_shader("shaders/waterocean.vs", "shaders/default_solid.fs");
      water_material = new material(vec4(0.0f, 0.0f, 1.0f, 1.0f), water_shader);
      //Setting up time
      atom_t atom_my_time = app_utils::get_atom("_time");
      float time_value = 0;
      uniform_time = water_material->add_uniform(&time_value, atom_my_time, GL_FLOAT, 1, param::stage_vertex);
      //Setting up time
      atom_t atom_number_waves = app_utils::get_atom("_number_waves");
      number_waves = 8;
      uniform_number_waves = water_material->add_uniform(&number_waves, atom_number_waves, GL_INT, 1, param::stage_vertex);

      //Creating the shape and adding it to the scene
      water_mesh_instance = app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(1000.0f, 0.5f, 1000.0f), ivec3(100, 1, 100), water_source),
        water_material,
        false, 0
        );
    }

  public:
    /// this is called when we construct the class before everything is initialised.
    Water3D(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      mouse_look_helper.init(this, 200.0f / 360.0f, false);
      fps_helper.init(this);
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->get_node()->translate(vec3(0, 4, 0));
      the_camera->set_far_plane(10000);

      strating = std::chrono::system_clock::now();
      time_per_frame = 20.0f;

      mat4t mat;

      mat.loadIdentity();
      mat.translate(0, -0.5f, 0);

      
      app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(100.0f, 0.5f, 100.0f), ivec3(100, 1, 100), source),
        new material(new image("assets/grass.jpg")),
        false, 0
        );

      set_up_water(mat);

      float player_height = 1.83f;
      float player_radius = 0.25f;
      float player_mass = 90.0f;

      mat.loadIdentity();
      mat.translate(-50, player_height*10.5f, -50);

      mesh_instance *mi = app_scene->add_shape(
        mat,
        new mesh_sphere(vec3(0), player_radius),
        new material(vec4(1, 0, 0, 1)),
        true, player_mass,
        new btCapsuleShape(0.25f, player_height)
        );
      player_node = mi->get_node();
    }

    void keyboard(){
      if (is_key_going_down('1')){
        water_mesh_instance->get_mesh()->set_mode(0);
      }
      else if (is_key_going_down('2')){
        water_mesh_instance->get_mesh()->set_mode(1);
      }
      else if (is_key_going_down('3')){
        water_mesh_instance->get_mesh()->set_mode(2);
      }
      else if (is_key_going_down('4')){
        water_mesh_instance->get_mesh()->set_mode(3);
      }
      else if (is_key_going_down('5')){
        water_mesh_instance->get_mesh()->set_mode(5);
      }
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      //Get input from keyboard!
      keyboard();

      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      //Things for the camera
      scene_node *camera_node = the_camera->get_node();
      mat4t &camera_to_world = camera_node->access_nodeToParent();
      mouse_look_helper.update(camera_to_world);

      fps_helper.update(player_node, camera_node);

      //Update time for the vertex shader
      std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
      std::chrono::duration<float> elapsed_seconds = now - strating;
      float new_time = elapsed_seconds.count();
      water_material->set_uniform(uniform_time, &new_time, sizeof(new_time));

      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);

      // draw the scene
      app_scene->render((float)vx / vy); 
    }
  };
}
