#version 330


uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 i_tc;
in vec3 Normal;  
in vec3 FragPos;  
in vec4 ic;

void main(void) {
   float ambientStrength = 0.7;
    vec3 ambient = ambientStrength* texture(tex,i_tc).rgb ;

 vec3 norm = normalize(Normal);
 vec3 lightDir = normalize(vec3(0, 0, -5) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse =ambientStrength *diff *texture(tex,i_tc).rgb;  

vec3 result = (ambient + diffuse) * texture(tex,i_tc).rgb;
	pixelColor=vec4(result, 1.0);
}
