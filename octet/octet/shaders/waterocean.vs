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
uniform float _type[8];
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

float wave(int i){
	float frequency = 2*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	vec2 direction = vec2(_dir_x[i],_dir_y[i]);
	float theta = dot(direction,vec2(pos.x,pos.z));
	float sin = ((sin(theta * frequency + _time * phase)+1)/2);
	return 2*_amplitude[i]*sin*sin*sin;
}

float waves(){
	float height = 0;
	for(int i=0; i<8; ++i)
		height += wave(i);
	return height;
}

vec3 wave_normal(int i){
	float frequency = 2*pi/_wave_lenght[i];
	float phase = _speed[i] * frequency;
	vec2 direction = vec2(_dir_x[i],_dir_y[i]);
	float theta = dot(direction,vec2(pos.x,pos.z));
	float cosx = cos(theta * frequency + _time * phase);
	float cosy = cos(theta * frequency + _time * phase);
	float sinx = (sin(theta * frequency + _time * phase)+1)/2;
	float siny = (sin(theta * frequency + _time * phase)+1)/2;
	float dDx = 3 * _amplitude[i] * direction.x * frequency * cosx * sinx * sinx;
	float dDy = 3 * _amplitude[i] * direction.y * frequency * cosy * siny * siny;
	return vec3(-dDx, -dDy, i);
}

vec3 waves_normal(){
	vec3 my_normal = vec3(0,0,0);
	for(int i=0; i<8; ++i)
		my_normal += wave_normal(i);
	return my_normal;
}

void main() {
  gl_Position = modelToProjection * (pos + vec3(0, waves(), 0));
  vec3 tnormal = (modelToCamera * vec4(waves_normal(), 0.0)).xyz;
  vec3 tpos = (modelToCamera * pos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = color;
  camera_pos_ = tpos;
  model_pos_ = pos.xyz;
}

