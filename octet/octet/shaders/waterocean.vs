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

float wave(){
	float amplitude = 2;
	float wavelenght = 5;
	float speed = 1;

	float frequency = 2*pi/wavelenght;
	float phase = speed * frequency;
	vec2 direction = vec2(0.5,1);
	float theta = dot(direction,vec2(pos.x,pos.z));
	//return amplitude*sin(tetha * frequency + _time * phase);
	return amplitude*sin(theta * frequency + _time * phase);
}

float waves(){
	float amplitude = 2;
	float wavelenght = 5;
	float speed = 1;
	float amplitude2 = 4;
	float wavelenght2 = 7;
	float speed2 = 2;
	
	float frequency = 2*pi/wavelenght;
	float frequency2 = 2*pi/wavelenght2;
	float phase = speed * frequency;
	float phase2 = speed2 * frequency2;
	vec2 direction = vec2(0,1);
	vec2 direction2 = vec2(1,0);
	float theta = dot(direction,vec2(pos.x,pos.z));
	float theta2 = dot(direction2,vec2(pos.x,pos.z));
	//return amplitude*sin(tetha * frequency + _time * phase);
	return amplitude*sin(theta2 * frequency2 + _time * phase2) + amplitude*sin(theta * frequency + _time * phase);
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

