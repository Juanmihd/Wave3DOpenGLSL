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
	return _amplitude[i]*sin(theta * frequency + _time * phase);
}

float waves(){
	float height = 0;

	for(int i=0; i<8; ++i)
		height += wave(i);

	return height;
}

void main() {
  gl_Position = modelToProjection * (pos + vec3(0, waves(), 0));
  vec3 tnormal = (modelToCamera * vec4(normal, 0.0)).xyz;
  vec3 tpos = (modelToCamera * pos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = color;
  camera_pos_ = tpos;
  model_pos_ = pos.xyz;
}

