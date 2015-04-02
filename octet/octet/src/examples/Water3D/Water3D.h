////////////////////////////////////////////////////////////////////////////////
//
// @author Juanmi Huertas
//
// This is the file containing the class that contains the application in openGL!
//

#include "..\..\..\open_source\AntTweakBar\include\AntTweakBar.h"
#include "WaterTerrain.h"
#include "tweakBarSetUp.h"
#include <chrono>
#include <ctime>

namespace octet {
  /// @brief This Water3D class is the application (based in Octet) to produce 3d Waves
  class Water3D : public app {
    enum {_NUM_WAVES = 8};
    // Information to render the scene
    bool skybox;
    ref<visual_scene> app_scene;
    mouse_look mouse_look_helper;
    ref<camera_instance> the_camera;
    ref<mesh_instance> water_mesh_instance;
    ref<mesh_instance> skysphere_instance;
    ref<mesh_instance> ground_instance;
    
    const char* file_name = "assets/wave3d/wave.conf";
    bool draw_twak_bar;
    int number_waves;
    int ground_rendered;
    float ball_y;
    TwBar* ui_main_bar;

    // Information regarding the waves and shaders of the waves
    example_geometry_source source;
    water_geometry_source water_source;
    ref<material> water_material;
    ref<param_uniform> uniform_time;
    ref<param_uniform> uniform_number_waves;
    ref<param_uniform> uniform_ground;
    ref<param_uniform> uniform_amplitudes;
    ref<param_uniform> uniform_speed;
    ref<param_uniform> uniform_wave_lenght;
    ref<param_uniform> uniform_dir_x;
    ref<param_uniform> uniform_dir_y;
    ref<param_uniform> uniform_steepness;
    ref<param_uniform> uniform_type;
    ref<param_uniform> uniform_atenuance;
    ref<scene_node> ball_node;
    WaveInfo waves_info;

    // Chrono to check the time to sen it to the shader
    std::chrono::time_point<std::chrono::system_clock> starting;
    float time_per_frame;

    // Info to open the file
    char* currentChar;
    dynarray<uint8_t> buffer;

    /// @brief Auxiliar function to read a single number representing an int
    int read_single_int(){
      int number = *currentChar - '0';
      currentChar += 2;
      return number;
    }

    /// @brief Auxiliar function to read a float from a char*
    /// To work, this float has to finish with an empty space ' '
    float read_float(){
      float number = 0;
      int decimals = -1;
      int negative = 1;
      if (*currentChar == '-'){
        negative = -1;
        ++currentChar;
      }
      while (*currentChar != ' '){
        if (*currentChar == '.'){
          decimals = 0;
        }
        else{
          number *= 10;
          number += *currentChar - '0';
          if (decimals >= 0) ++decimals;
        }
        ++currentChar;
      }
      ++currentChar;
      for (int i = 0; i < decimals; ++i)
        number /= 10;
      return number*negative;
    }

    void update_waves_uniforms(){
      water_material->set_uniform(uniform_number_waves, &number_waves, 1 * sizeof(int));
      water_material->set_uniform(uniform_amplitudes, waves_info.amplitude, 8 * sizeof(float));
      water_material->set_uniform(uniform_speed, waves_info.speed, 8 * sizeof(float));
      water_material->set_uniform(uniform_wave_lenght, waves_info.wave_length, 8 * sizeof(float));
      water_material->set_uniform(uniform_dir_x, waves_info.dir_x, 8 * sizeof(float));
      water_material->set_uniform(uniform_dir_y, waves_info.dir_y, 8 * sizeof(float));
      water_material->set_uniform(uniform_type, waves_info.type, 8 * sizeof(int));
      water_material->set_uniform(uniform_steepness, waves_info.steepness, 8 * sizeof(float));
      water_material->set_uniform(uniform_atenuance, waves_info.atenuance, 8 * sizeof(float));
      ball_node->loadIdentity();
      ball_node->translate(vec3(waves_info.dir_x[0], ball_y, waves_info.dir_y[0]));
    }

    /// @brief This function will read a file. It will load it from the parameter or from the 'default' file if none is given 
    void load_file(char* _file_name = nullptr){
      if (_file_name == nullptr)
        app_utils::get_url(buffer, file_name);
      else
        app_utils::get_url(buffer, file_name);

      currentChar = (char*)buffer.data();
      //Read number of waves (maximum 8!)
      number_waves = read_single_int();
      ++currentChar;
      //For each wave, read a line, and input that into thing
      for (int i = 0; i < number_waves; ++i){
        waves_info.amplitude[i] = read_float();
        waves_info.speed[i] = read_float();
        waves_info.wave_length[i] = read_float();
        waves_info.dir_x[i] = read_float();
        waves_info.dir_y[i] = read_float();
        waves_info.steepness[i] = read_float();
        waves_info.type[i] = read_single_int();
        waves_info.atenuance[i] = read_float();
        ++currentChar;
        ++currentChar;
      }
      //Update shader buffers!     
      update_waves_uniforms();
    }

    /// @brief This function helps to write a file with the wave information
    void save_file(char* _file_name = nullptr){
      std::ofstream file("wave_export.conf");
      if (file.is_open()){
        file << number_waves << "\n";
        for (int i = 0; i < number_waves; ++i){
          file << waves_info.amplitude[i] << " ";
          file << waves_info.speed[i] << " ";
          file << waves_info.wave_length[i] << " ";
          file << waves_info.dir_x[i] << " ";
          file << waves_info.dir_y[i] << " ";
          file << waves_info.steepness[i] << " ";
          file << waves_info.type[i] << " ";
          file << waves_info.atenuance[i] << " \n";
        }
        file.close();
      }
    }

    /// @brief This function sets up the shaders for the water
    void set_up_water(const mat4t& mat){
      //Setting up parameters of the shader!
      param_shader* water_shader;
      if (true){
        water_shader = new param_shader("shaders/waterocean.vs", "shaders/watersolid.fs");
        water_material = new material(new image("assets/skysphere.gif"), NULL, water_shader);
      }
      else{
        water_shader = new param_shader("shaders/waterocean.vs", "shaders/watersolid_2.fs");
        water_material = new material(vec4(0.25f,0.25f,0.25f), water_shader);
      }
      //Setting up time
      atom_t atom_my_time = app_utils::get_atom("_time");
      float time_value = 0;
      uniform_time = water_material->add_uniform(&time_value, atom_my_time, GL_FLOAT, 1, param::stage_vertex);
      //Setting up time
      number_waves = 8;
      atom_t atom_number_waves = app_utils::get_atom("_number_waves");
      uniform_number_waves = water_material->add_uniform(nullptr, atom_number_waves, GL_INT, 1, param::stage_vertex);
      water_material->set_uniform(uniform_number_waves, &number_waves, sizeof(int)); 
      //Zero value for waves
      for (int i = 0; i != 8; ++i){
        waves_info.amplitude[i] = 0.1f;
        waves_info.speed[i] = 0.1f;
        waves_info.wave_length[i] = 10.1f;
        waves_info.dir_x[i] = 10.0f;
        waves_info.dir_y[i] = 10.0f;
        waves_info.type[i] = 0;
        waves_info.steepness[i] = 0.0f;
        waves_info.atenuance[i] = 0.0f;
      }

      atom_t atom_amplitude = app_utils::get_atom("_amplitude");
      uniform_amplitudes = water_material->add_uniform(nullptr, atom_amplitude, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_amplitudes, waves_info.amplitude, 8 * sizeof(float)); //Thanks to Richard Fox for this line!
      atom_t atom_speed = app_utils::get_atom("_speed");
      uniform_speed = water_material->add_uniform(nullptr, atom_speed, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_speed, waves_info.speed, 8 * sizeof(float)); 
      atom_t atom_wave_lenght = app_utils::get_atom("_wave_lenght");
      uniform_wave_lenght = water_material->add_uniform(nullptr, atom_wave_lenght, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_wave_lenght, waves_info.wave_length, 8 * sizeof(float)); 
      atom_t atom_dir_x = app_utils::get_atom("_dir_x");
      uniform_dir_x = water_material->add_uniform(nullptr, atom_dir_x, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_dir_x, waves_info.dir_x, 8 * sizeof(float)); 
      atom_t atom_dir_y = app_utils::get_atom("_dir_y");
      uniform_dir_y = water_material->add_uniform(nullptr, atom_dir_y, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_dir_y, waves_info.dir_y, 8 * sizeof(float)); 
      atom_t atom_type = app_utils::get_atom("_type");
      uniform_type = water_material->add_uniform(nullptr, atom_type, GL_INT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_type, waves_info.type, 8 * sizeof(int)); 
      atom_t atom_steepness = app_utils::get_atom("_steepness");
      uniform_steepness = water_material->add_uniform(nullptr, atom_steepness, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_steepness, waves_info.steepness, 8 * sizeof(float));
      atom_t atom_atenuance = app_utils::get_atom("_atenuance");
      uniform_atenuance = water_material->add_uniform(nullptr, atom_atenuance, GL_FLOAT, 8, param::stage_vertex);
      water_material->set_uniform(uniform_atenuance, waves_info.atenuance, 8 * sizeof(float)); 
      atom_t atom_ground = app_utils::get_atom("_ground");
      uniform_ground = water_material->add_uniform(nullptr, atom_ground, GL_INT, 1, param::stage_vertex);
      water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));

      //Creating the shape and adding it to the scene
      water_mesh_instance = app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(100.0f, 0.0f, 100.0f), ivec3(400, 1, 400), source),
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
      ground_rendered = 1;
      ball_y = 45;
      //Information for the Twak Bar
      draw_twak_bar = false;
      TwInit(TW_OPENGL, NULL);
      ui_main_bar = TwNewBar("Waves");
      TwDefine(" Waves label='Waves config' color='216 20 35'  ");
      TwAddVarRW(ui_main_bar, "Number of waves", TW_TYPE_INT32, &number_waves, 
        " min=0 max=8 step=1 help='Number of waves to draw.' "); 
      
      set_up_tweak_bar(ui_main_bar, waves_info);

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
      node->rotate(45, vec3(1, 0, 0));
      node->rotate(-45, vec3(0, 1, 0));
      _light->set_color(vec4(1, 1, 1, 1));
      _light->set_kind(atom_directional);
      li->set_node(node);
      li->set_light(_light);
      app_scene->add_light_instance(li);
      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->get_node()->translate(vec3(0, 40, 0));
      the_camera->set_far_plane(10000);

      starting = std::chrono::system_clock::now();
      time_per_frame = 20.0f;

      mat4t mat;

      mat.loadIdentity();
      mat.translate(0, 30, 0);

      ground_instance = app_scene->add_shape(
        mat,
        new mesh_terrain(vec3(100.0f, 0.0f, 100.0f), ivec3(400, 1, 400), source),
        new material(new image("assets/grass.jpg")),
        false, 0
        );
      
     
      mat.loadIdentity();
      mat.translate(-50, 40, -50);

      ball_node = new scene_node;
      ball_node->translate(vec3(50, 45, 50));
      app_scene->add_mesh_instance(new mesh_instance(ball_node, new mesh_sphere(vec3(0, 0, 0), 5), new material(vec4(1, 0, 0, 1))));
      
      skysphere_instance = app_scene->add_mesh_instance(new mesh_instance(new scene_node, new mesh_sphere(vec3(0, 0, 0), 500), new material(new image("assets/skysphere.gif"))));

      mat.loadIdentity();
      mat.translate(0, 40, 0);
      set_up_water(mat);
      load_file();

      water_mesh_instance->get_mesh()->set_mode(0);
      skysphere_instance->set_flags(0);
      if (ground_rendered == 1){
        ground_instance->set_flags(0);
        ground_rendered = 0;
        water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
      }
    }

    void keyboard(){
      if (is_key_going_down('1')){
        water_mesh_instance->get_mesh()->set_mode(0);
        skysphere_instance->set_flags(0);
        if (ground_rendered == 1){
          ground_instance->set_flags(0);
          ground_rendered = 0;
          water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
        }
      }
      else if (is_key_going_down('2')){
        water_mesh_instance->get_mesh()->set_mode(1);
        skysphere_instance->set_flags(0);
        if (ground_rendered == 1){
          ground_instance->set_flags(0);
          ground_rendered = 0;
          water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
        }
      }
      else if (is_key_going_down('3')){
        water_mesh_instance->get_mesh()->set_mode(4);
        skysphere_instance->set_flags(0);
        if (ground_rendered == 1){
          ground_instance->set_flags(0);
          ground_rendered = 0;
          water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
        }
      }
      else if (is_key_going_down('4')){
        water_mesh_instance->get_mesh()->set_mode(4);
        skysphere_instance->set_flags(mesh_instance::flag_enabled);
        if (ground_rendered == 1){
          ground_instance->set_flags(0);
          ground_rendered = 0;
          water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
        }
      }
      else if (is_key_going_down('5')){
        water_mesh_instance->get_mesh()->set_mode(4);
        skysphere_instance->set_flags(mesh_instance::flag_enabled);
        if (ground_rendered == 0){
          ground_instance->set_flags(mesh_instance::flag_enabled);
          ground_rendered = 1;
          water_material->set_uniform(uniform_ground, &ground_rendered, sizeof(int));
        }
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
      if (is_key_going_down('0')){
        draw_twak_bar = !draw_twak_bar;
      }
      if (is_key_going_down('M')){
        save_file();
      }
    }

    void mouseinput(){
      //mouse control using x and y pos of mouse
      int x = 0;
      int y = 0;
      get_mouse_pos(x, y);
      //AntTweakBar stuff
      TwMouseMotion(x, y);
      if (is_key_going_down(key_lmb))
      {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
        //Update shader buffers!     
        update_waves_uniforms();
      }
      if (is_key_down(key_lmb)){
        //Update shader buffers!     
        update_waves_uniforms();
      }
      //NEED THIS
      if (!is_key_down(key_lmb) && get_prev_keys()[key_lmb] != 0)
      {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
        //Update shader buffers!     
        update_waves_uniforms();
      }
      //AntTweakBar stuff end
      if (is_key_going_down(key_rmb)){
        clear_absolute_mouse_movement();
      }
      if (is_key_down(key_rmb)){
        //Things for the camera
        scene_node *camera_node = the_camera->get_node();
        mat4t &camera_to_world = camera_node->access_nodeToParent();
        mouse_look_helper.update(camera_to_world);
      }
      else if (is_key_going_up(key_rmb)){
        int rx = 0, ry = 0;
        get_absolute_mouse_movement(rx, ry);
        mouse_look_helper.set_mouse_center(rx, ry);
      }
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      //Get input from keyboard!
      keyboard();

      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      TwWindowSize(vx, vy);
      app_scene->begin_render(vx, vy);
      mouseinput();


      //Update time for the vertex shader
      std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
      std::chrono::duration<float> elapsed_seconds = now - starting;
      float new_time = elapsed_seconds.count();
      //Check if new_time is close to size of float, and reset the "starting" counter
      water_material->set_uniform(uniform_time, &new_time, sizeof(new_time));
      ball_y += sin(new_time)*0.1f;
      ball_node->translate(vec3(0, sin(new_time)*0.1f, 0));
      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);

      // draw the scene
      app_scene->render((float)vx / vy);
      // tumble the box  (there is only one mesh instance)
      if (draw_twak_bar)
        TwDraw();
    }
  };
}
