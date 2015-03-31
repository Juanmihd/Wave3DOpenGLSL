//////////////////////////////////////////////////////////////////////////////////////////
//
// Juanmihd!
//
// This vertex shader will produce the waves simulation!
//
//////////////////////////////////////////////////////////////////////////////////////////

const float pi = 3.14159;
// uniforms for the waves
uniform float _time;
uniform int _number_waves;
uniform float _amplitude[8];
uniform float _speed[8];
uniform float _wave_lenght[8];
uniform float _dir_x[8];
uniform float _dir_y[8];
uniform int _type[8];
uniform float _steepness[8];
uniform float _atenuance[8];

// matrices
uniform mat4 modelToProjection;
uniform mat4 modelToCamera;

// attributes from vertex buffer
attribute vec4 pos;
attribute vec2 uv;
attribute vec3 normal;
attribute vec4 color;

// outputs
varying vec3 normal_;
varying vec2 uv_;
varying vec4 color_;
varying vec3 model_pos_;
varying vec3 camera_pos_;

vec2 get_direction(int i){
	if(_type[i] == 0){
		return vec2(_dir_x[i],_dir_y[i]);
	}else{
		vec2 direction = vec2(pos.x-_dir_x[i],pos.z-_dir_y[i]);
		return direction;
	}
}

vec3 wave(int i){
	vec2 odir = get_direction(i);
	vec2 real_pos = vec2(pos.x,pos.z);
	float distance = length(odir);
	float r_amplitude = _amplitude[i];
	if(_atenuance[i] > 0){
		if(distance < _atenuance[i]){
			r_amplitude = _amplitude[i]*(1-distance/_atenuance[i]);
		}else{
			r_amplitude = 0;
		}
	}
	vec2 dir = normalize(odir);
	float frequency = 2.0*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	float steepness = _steepness[i];
	float theta = 0;
	if(_type[i] == 0)
		theta = dot(dir,real_pos);
	else if(distance < 1) return vec3(0,0,0);
	else
		theta = dot(dir,odir);
	float mysin = sin(theta * frequency + _time * phase);
	float mycos = cos(theta * frequency + _time * phase);
	//return vec3(dir.x*steepness*r_amplitude*mycos,r_amplitude*mysin,dir.y*steepness*r_amplitude*mycos);
	return vec3(dir.x*steepness*r_amplitude*mycos,r_amplitude*mysin,dir.y*steepness*r_amplitude*mycos);
}

vec3 waves(){
	vec3 height = vec3(0,0,0);
	for(int i=0; i<_number_waves; ++i)
		height += wave(i);
	return height;
}

vec3 get_dxyz(int i){
	vec2 odir = get_direction(i);
	vec2 dir = normalize(odir);
	vec2 real_pos = vec2(pos.x,pos.z);
	float distance = length(odir);
	float r_amplitude = _amplitude[i];
	if(_atenuance[i] > 0){
		if(distance < _atenuance[i])
			r_amplitude = _amplitude[i]*(1-distance/_atenuance[i]);
		else
			r_amplitude = 0;
	}
	dir = normalize(dir);
	float frequency = 2.0*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	float steepness = _steepness[i];
	float theta = 0;
	if(_type[i] == 0)
		theta = dot(dir,real_pos);
	else if(distance < 1) return vec3(0.3,0.3,0.3);
	else
		theta = dot(dir,odir);
	float mycos = cos(theta * frequency + _time * phase);
	float mysin = cos(theta * frequency + _time * phase);
	float dx = 2 * r_amplitude * dir.x * frequency * mycos;
	float dy = 2 * r_amplitude * dir.y * frequency * mycos;
	float dheight = steepness * r_amplitude * frequency * mysin;
	return vec3(dx,dheight,dy);
}

vec3 waves_normal(){
	vec3 dxy = vec3(0.0,0.0,0.0);
	for(int i=0; i<_number_waves; ++i){
		dxy -= get_dxyz(i);
	}
	vec3 tnormal = vec3(dxy.x, 1+dxy.y, dxy.z);
	return normalize(tnormal);
}

void main() {
  vec3 mywave = waves();
  vec4 temppos = vec4(pos.x+mywave.x,  mywave.y, pos.z+mywave.z, pos.w);
  gl_Position = modelToProjection * temppos;
  vec3 tnormal = (modelToCamera * vec4(waves_normal(),0.0)).xyz;
  vec3 tpos = (modelToCamera * temppos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = color;
  camera_pos_ = tpos;
  model_pos_ = temppos.xyz;
}

