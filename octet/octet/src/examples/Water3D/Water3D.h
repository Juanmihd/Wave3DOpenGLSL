////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//


#include <chrono>
#include <ctime>
#include "WaterTerrain.h"
//#include "mesh_huge_plane.h"

namespace octet {
  /// Scene containing a box with octet.
  class Water3D : public app {
    enum {_NUM_WAVES = 8};
    // scene for drawing box
    ref<visual_scene> app_scene;

    mouse_look mouse_look_helper;

    ref<camera_instance> the_camera;

    example_geometry_source source;
    water_geometry_source water_source;

    ref<material> water_material;
    ref<param_uniform> uniform_time;
    ref<param_uniform> uniform_number_waves;
    ref<param_uniform> uniform_amplitudes;
    ref<param_uniform> uniform_speed;
    ref<param_uniform> uniform_wave_lenght;
    ref<param_uniform> uniform_dir_x;
    ref<param_uniform> uniform_dir_y;
    ref<param_uniform> uniform_steepness;
    ref<param_uniform> uniform_type;
    ref<param_uniform> uniform_atenuance;
    ref<mesh_instance> water_mesh_instance;
    ref<mesh_instance> skysphere_instance;
    ref<mesh_instance> ground_instance;
    ref<scene_node> ball_node;
    WaveInfo waves_info;
    bool skybox;

    std::chrono::time_point<std::chrono::system_clock> strating;
    float time_per_frame;
    int number_waves;

    void set_up_water(const mat4t& mat){
      //Setting up parameters of the shader!
      param_shader* water_shader;
      if (!skybox)
        water_shader = new param_shader("shaders/waterocean.vs", "shaders/watersolid.fs");
      else
        water_shader = new param_shader("shaders/waterocean.vs", "shaders/watersolid.fs");
      water_material = new material(new image("assets/skysphere.gif"),NULL, water_shader);
      //Setting up time
      atom_t atom_my_time = app_utils::get_atom("_time");
      float time_value = 0;
      uniform_time = water_material->add_uniform(&time_value, atom_my_time, GL_FLOAT, 1, param::stage_vertex);
      //Setting up time
      atom_t atom_number_waves = app_utils::get_atom("_number_waves");
      number_waves = 8;
      uniform_number_waves = water_material->add_uniform(&number_waves, atom_number_waves, GL_INT, 1, param::stage_vertex);
      //Setting up waves
      for (int i = 0; i != 8; ++i){
        waves_info.amplitude[i] = (1.0f + i / 8.0f)*0.25;
        waves_info.speed[i] = 0.5f + i / 40.0f;
        waves_info.wave_length[i] = 6.5f + i / 5.0f;
        waves_info.dir_x[i] = i / 4.0f;
        waves_info.dir_y[i] = 1 - i / 8.0f;
        waves_info.type[i] = 0;
        waves_info.steepness[i] =0.0f;
        waves_info.atenuance[i] = -1.0f;
      }
      waves_info.amplitude[0] = 2.0f/number_waves;
      waves_info.speed[0] = 1.5f;
      waves_info.wave_length[0] = 10.5f;
      waves_info.steepness[0] = 1.0f;
      waves_info.dir_x[0] = 0.0f;
      waves_info.dir_y[0] = 0.0f;
      waves_info.dir_x[1] = 0.0f;
      waves_info.dir_y[1] = 0.0f;
      waves_info.type[0] = 1;
      waves_info.type[1] = 1;
      //waves_info.type[4] = 1;
      //Qi = 1/(wi Ai )
      waves_info.steepness[0] = 0;// 0.8f / (waves_info.amplitude[0] * 2.0f*3.141592f / waves_info.wave_length[0]);
     atom_t atom_amplitude = app_utils::get_atom("_amplitude");
      uniform_amplitudes = water_material->add_uniform(nullptr, atom_amplitude, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_amplitudes, waves_info.amplitude, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_speed = app_utils::get_atom("_speed");
      uniform_speed = water_material->add_uniform(nullptr, atom_speed, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_speed, waves_info.speed, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_wave_lenght = app_utils::get_atom("_wave_lenght");
      uniform_wave_lenght = water_material->add_uniform(nullptr, atom_wave_lenght, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_wave_lenght, waves_info.wave_length, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_dir_x = app_utils::get_atom("_dir_x");
      uniform_dir_x = water_material->add_uniform(nullptr, atom_dir_x, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_dir_x, waves_info.dir_x, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_dir_y = app_utils::get_atom("_dir_y");
      uniform_dir_y = water_material->add_uniform(nullptr, atom_dir_y, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_dir_y, waves_info.dir_y, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_type = app_utils::get_atom("_type");
      uniform_type = water_material->add_uniform(nullptr, atom_type, GL_INT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_type, waves_info.type, 8 * sizeof(int)); //Thanks to Richard Fox for this bit!
      atom_t atom_steepness = app_utils::get_atom("_steepness");
      uniform_steepness = water_material->add_uniform(nullptr, atom_steepness, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_steepness, waves_info.steepness, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!
      atom_t atom_atenuance = app_utils::get_atom("_atenuance");
      uniform_atenuance = water_material->add_uniform(nullptr, atom_atenuance, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_atenuance, waves_info.atenuance, 8 * sizeof(float)); //Thanks to Richard Fox for this bit!

      //Creating the shape and adding it to the scene
      water_mesh_instance = app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(100.0f, 0.5f, 100.0f), ivec3(200, 1, 200), water_source),
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
      skybox = false;
      mouse_look_helper.init(this, 200.0f / 360.0f, false);
      app_scene = new visual_scene();
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      light *_light = new light();
      light_instance *li = new light_instance();
      node->translate(vec3(100, 100, 100));
      node->rotate(45, vec3(1, 0, 0));
      node->rotate(45, vec3(0, 1, 0));
      _light->set_color(vec4(1, 1, 1, 1));
      _light->set_kind(atom_directional);
      li->set_node(node);
      li->set_light(_light);
      app_scene->add_light_instance(li);
      node = new scene_node();
      app_scene->add_child(node);
      _light = new light();
      li = new light_instance();
      node->translate(vec3(-100, 100, -100));
      node->rotate(-45, vec3(1, 0, 0));
      node->rotate(45, vec3(0, 1, 0));
      _light->set_color(vec4(1, 1, 1, 1));
      _light->set_kind(atom_directional);
      li->set_node(node);
      li->set_light(_light);
      app_scene->add_light_instance(li);
      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->get_node()->translate(vec3(0, 40, 0));
      the_camera->set_far_plane(10000);

      strating = std::chrono::system_clock::now();
      time_per_frame = 20.0f;

      mat4t mat;

      mat.loadIdentity();
      mat.translate(0, 0, 0);

      ground_instance = app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(100.0f, 2.5f, 100.0f), ivec3(180, 1, 180), source),
        new material(new image("assets/grass.jpg")),
        false, 0
        );
      
     
      mat.loadIdentity();
      mat.translate(-50, 40, -50);

      ball_node = new scene_node;
      ball_node->translate(vec3(0, 45, 0));
      app_scene->add_mesh_instance(new mesh_instance(ball_node, new mesh_sphere(vec3(0, 0, 0), 5), new material(vec4(1, 0, 0, 1))));
      
      skysphere_instance = app_scene->add_mesh_instance(new mesh_instance(new scene_node, new mesh_sphere(vec3(0, 0, 0), 500), new material(new image("assets/skysphere.gif"))));

      mat.loadIdentity();
      mat.translate(0, 40, 0);
      set_up_water(mat);
     // app_scene->add_mesh_instance(new mesh_instance(new scene_node, new mesh_box(vec3(500,500,500)), new material(new image("assets/skybox.gif"))));
    }

    void keyboard(){
      if (is_key_going_down('1')){
        water_mesh_instance->get_mesh()->set_mode(0);
        skysphere_instance->set_flags(0);
        ground_instance->set_flags(0);
      }
      else if (is_key_going_down('2')){
        water_mesh_instance->get_mesh()->set_mode(1);
        skysphere_instance->set_flags(0);
        ground_instance->set_flags(0);
      }
      else if (is_key_going_down('3')){
        water_mesh_instance->get_mesh()->set_mode(4);
        skysphere_instance->set_flags(0);
        ground_instance->set_flags(0);
      }
      else if (is_key_going_down('4')){
        water_mesh_instance->get_mesh()->set_mode(4);
        skysphere_instance->set_flags(mesh_instance::flag_enabled);
        ground_instance->set_flags(mesh_instance::flag_enabled);
      }
      else if (is_key_down('W')){
        the_camera->get_node()->translate(vec3(0, 0, -1));
      }
      else if (is_key_down('S')){
        the_camera->get_node()->translate(vec3(0, 0, 1));
      }
      else if (is_key_down('A')){
        the_camera->get_node()->translate(vec3(-1, 0, 0));
      }
      else if (is_key_down('D')){
        the_camera->get_node()->translate(vec3(1, 0, 0));
      }
      else if (is_key_down(key_up)){
        ball_node->translate(vec3(1, 0, 0));
        waves_info.dir_x[0] += 1.0f;
        water_material->set_uniform(uniform_dir_x, waves_info.dir_x, 8 * sizeof(float));
        printf("Center: x%f, y%f\n", waves_info.dir_x[0], waves_info.dir_y[0]);
      }
      else if (is_key_down(key_down)){
        ball_node->translate(vec3(-1, 0, 0));
        waves_info.dir_x[0] -= 1.0f;
        water_material->set_uniform(uniform_dir_x, waves_info.dir_x, 8 * sizeof(float));
        printf("Center: x%f, y%f\n", waves_info.dir_x[0], waves_info.dir_y[0]);
      }
      else if (is_key_down(key_left)){
        ball_node->translate(vec3(0, 0, 1));
        waves_info.dir_y[0] += 1.0f;
        water_material->set_uniform(uniform_dir_y, waves_info.dir_y, 8 * sizeof(float));
        printf("Center: x%f, y%f\n", waves_info.dir_x[0], waves_info.dir_y[0]);
      }
      else if (is_key_down(key_right)){
        ball_node->translate(vec3(0, 0, -1));
        waves_info.dir_y[0] -= 1.0f;
        water_material->set_uniform(uniform_dir_y, waves_info.dir_y, 8 * sizeof(float));
        printf("Center: x%f, y%f\n", waves_info.dir_x[0], waves_info.dir_y[0]);
      }
      else if (is_key_down('R')){
        waves_info.steepness[0] += 0.1f;
        water_material->set_uniform(uniform_steepness, waves_info.steepness, 8 * sizeof(float));
      }
      else if (is_key_down('F')){
        waves_info.steepness[0] -= 0.1f;
        water_material->set_uniform(uniform_steepness, waves_info.steepness, 8 * sizeof(float));
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


      //Update time for the vertex shader
      std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
      std::chrono::duration<float> elapsed_seconds = now - strating;
      float new_time = elapsed_seconds.count();
      water_material->set_uniform(uniform_time, &new_time, sizeof(new_time));
      ball_node->translate(vec3(0, sin(new_time)*0.1f, 0));
      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
