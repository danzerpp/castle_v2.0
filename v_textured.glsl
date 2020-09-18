#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location = 1) in vec3 aNormal;
layout (location=2) in vec2 texCoord; //wspó³rzêdne teksturowania


//Zmienne interpolowane
out vec2 i_tc;
out vec3 Normal;
out vec3 FragPos;

void main(void) {
    FragPos = vec3(M * vertex);
    gl_Position=P*V*M*vertex;
    Normal = mat3(transpose(inverse(M))) * aNormal;
    i_tc=texCoord;

}
