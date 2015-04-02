#ifndef _TWEAKBARSETUP_H_INCLUDED_
#define _TWEAKBARSETUP_H_INCLUDED_

namespace octet{
  void set_up_tweak_bar(TwBar* ui_main_bar, WaveInfo &waves_info){
    //Wave 1
    TwAddVarRW(ui_main_bar, "Type1", TW_TYPE_INT32, &waves_info.type[0],
      " label='Type of wave' group='Wave 1' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude1", TW_TYPE_FLOAT, &waves_info.amplitude[0],
      " label='Amplitude' group='Wave 1' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length1", TW_TYPE_FLOAT, &waves_info.wave_length[0],
      " label='Wave length' group='Wave 1' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed1", TW_TYPE_FLOAT, &waves_info.speed[0],
      " label='Speed' group='Wave 1' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx1", TW_TYPE_FLOAT, &waves_info.dir_x[0],
      " label='Dir x (or center x)' group='Wave 1' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry1", TW_TYPE_FLOAT, &waves_info.dir_y[0],
      " label='Dir y (or center y)' group='Wave 1' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness1", TW_TYPE_FLOAT, &waves_info.steepness[0],
      " label='Steepness' group='Wave 1' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance1", TW_TYPE_FLOAT, &waves_info.atenuance[0],
      " label='Atenuance' group='Wave 1' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 2
    TwAddVarRW(ui_main_bar, "Type2", TW_TYPE_INT32, &waves_info.type[1],
      " label='Type of wave' group='Wave 2' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude2", TW_TYPE_FLOAT, &waves_info.amplitude[1],
      " label='Amplitude' group='Wave 2' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length2", TW_TYPE_FLOAT, &waves_info.wave_length[1],
      " label='Wave length' group='Wave 2' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed2", TW_TYPE_FLOAT, &waves_info.speed[1],
      " label='Speed' group='Wave 2' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx2", TW_TYPE_FLOAT, &waves_info.dir_x[1],
      " label='Dir x (or center x)' group='Wave 2' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry2", TW_TYPE_FLOAT, &waves_info.dir_y[1],
      " label='Dir y (or center y)' group='Wave 2' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness2", TW_TYPE_FLOAT, &waves_info.steepness[1],
      " label='Steepness' group='Wave 2' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance2", TW_TYPE_FLOAT, &waves_info.atenuance[1],
      " label='Atenuance' group='Wave 2' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 3
    TwAddVarRW(ui_main_bar, "Type4", TW_TYPE_INT32, &waves_info.type[3],
      " label='Type of wave' group='Wave 4' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude4", TW_TYPE_FLOAT, &waves_info.amplitude[3],
      " label='Amplitude' group='Wave 4' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length4", TW_TYPE_FLOAT, &waves_info.wave_length[3],
      " label='Wave length' group='Wave 4' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed4", TW_TYPE_FLOAT, &waves_info.speed[3],
      " label='Speed' group='Wave 4' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx4", TW_TYPE_FLOAT, &waves_info.dir_x[3],
      " label='Dir x (or center x)' group='Wave 4' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry4", TW_TYPE_FLOAT, &waves_info.dir_y[3],
      " label='Dir y (or center y)' group='Wave 4' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness4", TW_TYPE_FLOAT, &waves_info.steepness[3],
      " label='Steepness' group='Wave 4' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance4", TW_TYPE_FLOAT, &waves_info.atenuance[3],
      " label='Atenuance' group='Wave 4' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 4
    TwAddVarRW(ui_main_bar, "Type5", TW_TYPE_INT32, &waves_info.type[4],
      " label='Type of wave' group='Wave 5' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude5", TW_TYPE_FLOAT, &waves_info.amplitude[4],
      " label='Amplitude' group='Wave 5' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length5", TW_TYPE_FLOAT, &waves_info.wave_length[4],
      " label='Wave length' group='Wave 5' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed5", TW_TYPE_FLOAT, &waves_info.speed[4],
      " label='Speed' group='Wave 5' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx5", TW_TYPE_FLOAT, &waves_info.dir_x[4],
      " label='Dir x (or center x)' group='Wave 5' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry5", TW_TYPE_FLOAT, &waves_info.dir_y[4],
      " label='Dir y (or center y)' group='Wave 5' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness5", TW_TYPE_FLOAT, &waves_info.steepness[4],
      " label='Steepness' group='Wave 5' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance5", TW_TYPE_FLOAT, &waves_info.atenuance[4],
      " label='Atenuance' group='Wave 5' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 5
    TwAddVarRW(ui_main_bar, "Type6", TW_TYPE_INT32, &waves_info.type[5],
      " label='Type of wave' group='Wave 6' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude6", TW_TYPE_FLOAT, &waves_info.amplitude[5],
      " label='Amplitude' group='Wave 6' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length6", TW_TYPE_FLOAT, &waves_info.wave_length[5],
      " label='Wave length' group='Wave 6' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed6", TW_TYPE_FLOAT, &waves_info.speed[5],
      " label='Speed' group='Wave 6' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx6", TW_TYPE_FLOAT, &waves_info.dir_x[5],
      " label='Dir x (or center x)' group='Wave 6' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry6", TW_TYPE_FLOAT, &waves_info.dir_y[5],
      " label='Dir y (or center y)' group='Wave 6' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness6", TW_TYPE_FLOAT, &waves_info.steepness[5],
      " label='Steepness' group='Wave 6' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance6", TW_TYPE_FLOAT, &waves_info.atenuance[5],
      " label='Atenuance' group='Wave 6' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 6
    TwAddVarRW(ui_main_bar, "Type6", TW_TYPE_INT32, &waves_info.type[5],
      " label='Type of wave' group='Wave 6' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude6", TW_TYPE_FLOAT, &waves_info.amplitude[5],
      " label='Amplitude' group='Wave 6' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length6", TW_TYPE_FLOAT, &waves_info.wave_length[5],
      " label='Wave length' group='Wave 6' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed6", TW_TYPE_FLOAT, &waves_info.speed[5],
      " label='Speed' group='Wave 6' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx3", TW_TYPE_FLOAT, &waves_info.dir_x[5],
      " label='Dir x (or center x)' group='Wave 6' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry6", TW_TYPE_FLOAT, &waves_info.dir_y[5],
      " label='Dir y (or center y)' group='Wave 6' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness6", TW_TYPE_FLOAT, &waves_info.steepness[5],
      " label='Steepness' group='Wave 6' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance6", TW_TYPE_FLOAT, &waves_info.atenuance[5],
      " label='Atenuance' group='Wave 6' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 7
    TwAddVarRW(ui_main_bar, "Type7", TW_TYPE_INT32, &waves_info.type[6],
      " label='Type of wave' group='Wave 7' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude7", TW_TYPE_FLOAT, &waves_info.amplitude[6],
      " label='Amplitude' group='Wave 7' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length7", TW_TYPE_FLOAT, &waves_info.wave_length[6],
      " label='Wave length' group='Wave 7' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed7", TW_TYPE_FLOAT, &waves_info.speed[6],
      " label='Speed' group='Wave 7' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx7", TW_TYPE_FLOAT, &waves_info.dir_x[6],
      " label='Dir x (or center x)' group='Wave 7' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry7", TW_TYPE_FLOAT, &waves_info.dir_y[6],
      " label='Dir y (or center y)' group='Wave 7' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness7", TW_TYPE_FLOAT, &waves_info.steepness[6],
      " label='Steepness' group='Wave 7' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance7", TW_TYPE_FLOAT, &waves_info.atenuance[6],
      " label='Atenuance' group='Wave 7' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
    //Wave 8
    TwAddVarRW(ui_main_bar, "Type8", TW_TYPE_INT32, &waves_info.type[7],
      " label='Type of wave' group='Wave 8' min=0 max=1 step=1 help='0 circular, 1 linear' ");
    TwAddVarRW(ui_main_bar, "Amplitude8", TW_TYPE_FLOAT, &waves_info.amplitude[7],
      " label='Amplitude' group='Wave 8' min=0.1 max=10.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "length8", TW_TYPE_FLOAT, &waves_info.wave_length[7],
      " label='Wave length' group='Wave 8' min=0.01 max=50.0 step=0.01 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Speed8", TW_TYPE_FLOAT, &waves_info.speed[7],
      " label='Speed' group='Wave 8' min=-5.0 max=5.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Dirx8", TW_TYPE_FLOAT, &waves_info.dir_x[7],
      " label='Dir x (or center x)' group='Wave 8' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Diry8", TW_TYPE_FLOAT, &waves_info.dir_y[7],
      " label='Dir y (or center y)' group='Wave 8' min=-1000.0 max=1000.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Steepness8", TW_TYPE_FLOAT, &waves_info.steepness[7],
      " label='Steepness' group='Wave 8' min=-1.0 max=1.0 step=0.1 help='This is the wave currently editing' ");
    TwAddVarRW(ui_main_bar, "Atenuance8", TW_TYPE_FLOAT, &waves_info.atenuance[7],
      " label='Atenuance' group='Wave 8' min=0.0 max=100.0 step=2.0 help='This is the wave currently editing' ");
  }

}

#endif