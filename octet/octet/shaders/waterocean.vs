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
uniform float _cen_x[8];
uniform float _cen_y[8];

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


vec3 wave(int i){
	vec2 dir = vec2(_dir_x[i],_dir_y[i]);
	float frequency = 2.0*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	float steepness = 1.5;
	float theta = dot(normalize(dir),vec2(pos.x,pos.z));
	float mysin = sin(theta * frequency + _time * phase);
	float mycos = cos(theta * frequency + _time * phase);
	return vec3(dir.x*steepness*_amplitude[i]*mycos,_amplitude[i]*mysin,dir.y*steepness*_amplitude[i]*mycos);
}

vec3 waves(){
	vec3 height = vec3(0,0,0);
	for(int i=0; i<_number_waves; ++i)
		height += wave(i);
	return height;
}

vec3 get_dxyz(int i){
	vec2 dir = vec2(_dir_x[i],_dir_y[i]);
	float frequency = 2.0*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	float steepness = 1.5;
	float theta = dot(normalize(dir),vec2(pos.x,pos.z));
	float mycos = cos(theta * frequency + _time * phase);
	float mysin = cos(theta * frequency + _time * phase);
	float dx = _amplitude[i] * dir.x * frequency * mycos;
	float dy = _amplitude[i] * dir.y * frequency * mycos;
	float dheight = steepness * _amplitude[i] * frequency * mysin;
	return vec3(dx,dheight,dy);
}

vec3 waves_normal(){
	vec3 dxy = vec3(0.0,0.0,0.0);
	for(int i=0; i<_number_waves; ++i){
		dxy += get_dxyz(i);
	}
	vec3 tnormal = vec3(-dxy.x, 1-dxy.y, -dxy.z);
	return tnormal;
}

void main() {
  vec3 mywave = waves();
  vec4 temppos = vec4(pos.x+mywave.x, mywave.y, pos.z+mywave.z, pos.w);
  gl_Position = modelToProjection * temppos;
  vec3 tnormal = waves_normal();
  vec3 tpos = (modelToCamera * temppos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = color;
  camera_pos_ = tpos;
  model_pos_ = temppos.xyz;
}

