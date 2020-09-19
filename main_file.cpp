/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "myCube.h"
#include <glad/glad.h>
#include "Bridge.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Square.h"
#include "groundSquare.h"
#include "Object_Loader.h"
#include "LoadModel.h"

float speed_x = 0.5f;//[radiany/s]
float speed_y = 0;//[radiany/s]
float water_speed = 1;
GLuint tex;
GLuint bridgeTex;
GLuint sandTex;
GLuint waterTex;
GLuint groundTex;
GLuint castleGroundTex;
GLuint FlagTex;
float yaw = 90;
float pitch = 0;
//Bridge
float aspectRatio = 1;
objl::Loader loader;
float angle_bridge = 0;
bool getBridgeDown = false;
float bridgeSpeed = 0;
float time =0;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400, lastY = 300;

//Shader ourShader("shader.vs", "shader.fs");
//Model ourModel("Tree.obj");
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.25f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		yaw -= 10;
		//pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		yaw += 10;
		//pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pitch += 10;
		//pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{

		pitch -= 10;
		//pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		if (getBridgeDown)
		{
			getBridgeDown = false;
			bridgeSpeed = 30;
		}
		else
		{
			getBridgeDown = true;
			bridgeSpeed = -30;

		}
	}

}


void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}



GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 1, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
	tex = readTexture("whiteBricks.png");
	bridgeTex = readTexture("bridgeTexture.png");
	sandTex = readTexture("stone-wall.png");
	waterTex = readTexture("waterTexture.png");
	groundTex = readTexture("grass.png");
	castleGroundTex = readTexture("cobblestone.png");
	FlagTex = readTexture("flag.png");
	glm::vec3 direction;
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	//loads model 

}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex);
}

//void ptica2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
//	//Przykładowe tablice dla tego zadania - możliwości jest bardzo dużo
//	float birdVertices[12];
//	bridvertices += 1.0;
//
//	float birdColors[] = {
//		0,1,0,1,
//		0,1,0,1,
//		0,1,0,1,
//
//		0,1,0,1,
//		0,1,0,1,
//		0,1,0,1,
//	};
//
//	spColored->use(); //Aktywuj program cieniujący
//
//	glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
//	glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
//	glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
//
//
//	glEnableVertexAttribArray(spColored->a("vertex"));
//	glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, birdVertices); //Współrzędne wierzchołków bierz z tablicy birdVertices
//
//	glEnableVertexAttribArray(spColored->a("color"));
//	glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, birdColors); //Współrzędne wierzchołków bierz z tablicy birdColors
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableVertexAttribArray(spColored->a("vertex"));
//	glDisableVertexAttribArray(spColored->a("color"));
//
//	//spLambert->use(); //Aktyeuj program cieniujący
//	//glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
//	//glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
//	//glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu
//
//	// M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
//	//M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));
//	//glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
//	//Models::torus.drawSolid(); //Narysuj obiekt
//
//}

void drawGround(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	//Przykładowe tablice dla tego zadania - możliwości jest bardzo dużo

	spTextured->use(); //Aktywuj program cieniujący
	
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myGroundSquareVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myGroundSquareTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myGroundSquareVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}




void kostka(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

	spColored->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spColored->a("vertex"));
	glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy birdVertices

	glEnableVertexAttribArray(spColored->a("color"));
	glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, myCubeColors); //Współrzędne wierzchołków bierz z tablicy birdColors

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spColored->a("vertex"));
	glDisableVertexAttribArray(spColored->a("color"));
}

void drawBridge(glm::mat4 P, glm::mat4 V, glm::mat4 M) 
{
	//spColored->use(); //Aktywuj program cieniujący
	//glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	//glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	//glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	//glEnableVertexAttribArray(spColored->a("vertex"));
	//glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, myBridgeVertices); //Współrzędne wierzchołków bierz z tablicy birdVertices

	//glEnableVertexAttribArray(spColored->a("color"));
	//glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, myCubeColors); //Współrzędne wierzchołków bierz z tablicy birdColors

	//glDrawArrays(GL_TRIANGLES, 0, myBridgeVertexCount);

	//glDisableVertexAttribArray(spColored->a("vertex"));
	//glDisableVertexAttribArray(spColored->a("color"));


	//Tablica ta raczej powinna znaleźć się w pliku myCube.h, ale umieściłem ją tutaj, żeby w tej procedurze zawrzeć (prawie) całe rozwiązanie zadania
	//Reszta to wczytanie tekstury - czyli kawałki kodu, które trzeba przekopiować ze slajdów
	float myCubeTexCoords[] = {
		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
	};

	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myBridgeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bridgeTex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

void texKostka(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	//Tablica ta raczej powinna znaleźć się w pliku myCube.h, ale umieściłem ją tutaj, żeby w tej procedurze zawrzeć (prawie) całe rozwiązanie zadania
	//Reszta to wczytanie tekstury - czyli kawałki kodu, które trzeba przekopiować ze slajdów
	float myCubeTexCoords[] = {
		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
	};


	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

void texCastleGround(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	//Tablica ta raczej powinna znaleźć się w pliku myCube.h, ale umieściłem ją tutaj, żeby w tej procedurze zawrzeć (prawie) całe rozwiązanie zadania
	//Reszta to wczytanie tekstury - czyli kawałki kodu, które trzeba przekopiować ze slajdów
	float myCubeTexCoords[] = {
		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
	};


	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, castleGroundTex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}


//void texKostka2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
//	//Tablica ta raczej powinna znaleźć się w pliku myCube.h, ale umieściłem ją tutaj, żeby w tej procedurze zawrzeć (prawie) całe rozwiązanie zadania
//	//Reszta to wczytanie tekstury - czyli kawałki kodu, które trzeba przekopiować ze slajdów
//	float myCubeTexCoords[] = {
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//
//		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
//		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,
//	};

	//float myCubeNormals[] = {
	//	0,0,-1,0, 0,0,-1,0, 0,0,-1,0,
	//	0,0,-1,0, 0,0,-1,0, 0,0,-1,0,

	//	0,0, 1,0, 0,0, 1,0, 0,0, 1,0,
	//	0,0, 1,0, 0,0, 1,0, 0,0, 1,0,

	//	0,-1,0,0, 0,-1,0,0, 0,-1,0,0,
	//	0,-1,0,0, 0,-1,0,0, 0,-1,0,0,

	//	0, 1,0,0, 0, 1,0,0, 0, 1,0,0,
	//	0, 1,0,0, 0, 1,0,0, 0, 1,0,0,

	//	-1,0,0,0, -1,0,0,0, -1,0,0,0,
	//	-1,0,0,0, -1,0,0,0, -1,0,0,0,

	//	1,0,0,0, 1,0,0,0, 1,0,0,0,
	//	1,0,0,0, 1,0,0,0, 1,0,0,0,
	//};

	//spLambertTextured->use(); //Aktywuj program cieniujący

	//glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	//glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	//glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	//glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	//glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	//glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	//glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	//glEnableVertexAttribArray(spLambertTextured->a("normal"));
	//glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals); //Wektory normalne bierz z tablicy myCubeNormals

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, tex);
	//glUniform1i(spLambertTextured->u("tex"), 0);

	//glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	//glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	//glDisableVertexAttribArray(spLambertTextured->a("color"));
	//glDisableVertexAttribArray(spLambertTextured->a("normal"));
//}

void drawSand(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, mySquareVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, mySquareTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sandTex);
	glUniform1i(spTextured->u("sandTex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, mySquareVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

void	drawWater(glm::mat4 P, glm::mat4 V, glm::mat4 M, float angle) {

	int const count = 1440;
	float myFlat[count];
	int vertice = 0;
	float myFlatColors[count];
	float myFlatTexture[count/2];
	int texVertice = 0;

	for (float i = -30; i < 30; i=i+1.0f)
	{
		// pierwszy trójkąt


		myFlat[vertice] = i + 1.0f;
		myFlat[vertice + 1] = 0.5*cos(water_speed*(time + (30 + i + 1.0f)));
		myFlat[vertice + 2] = -2;
		myFlat[vertice + 3] = 1;

		myFlat[vertice +4] = i;
		myFlat[vertice + 5] = 0.5*cos(water_speed*(time + (30 + i)));
		myFlat[vertice + 6] = 2;
		myFlat[vertice + 7] = 1;

		myFlat[vertice + 8] = i;
		myFlat[vertice + 9] = 0.5*cos(water_speed*(time + (30 + i)));
		myFlat[vertice + 10] = -2;
		myFlat[vertice + 11] = 1;

		// drugi trójkąt


		myFlat[vertice + 12] = i + 1.0f;
		myFlat[vertice + 13] = 0.5*cos(water_speed*(time + (30 + i + 1.0f)));
		myFlat[vertice + 14] = -2;
		myFlat[vertice + 15] = 1;

		myFlat[vertice + 16] = i + 1.0f;
		myFlat[vertice + 17] = 0.5*cos(water_speed*(time + (30 + i + 1.0f)));;
		myFlat[vertice + 18] = 2;
		myFlat[vertice + 19] = 1;


		myFlat[vertice + 20] = i;
		myFlat[vertice + 21] = 0.5*cos(water_speed*(time + (30 + i)));
		myFlat[vertice + 22] = 2;
		myFlat[vertice + 23] = 1;
		

		myFlatTexture[texVertice] = 0;


		myFlatColors[vertice] = 0;
		myFlatColors[vertice + 1] = 0;
		myFlatColors[vertice + 2] = 1;
		myFlatColors[vertice + 3] = 1;


		myFlatColors[vertice +4] = 0;
		myFlatColors[vertice + 5] = 0;
		myFlatColors[vertice + 6] = 1;
		myFlatColors[vertice + 7] = 1;

		

		myFlatColors[vertice + 8] = 0;
		myFlatColors[vertice + 9] = 0;
		myFlatColors[vertice + 10] = 1;
		myFlatColors[vertice + 11] = 1;

	

		myFlatColors[vertice + 12] = 0;
		myFlatColors[vertice + 13] = 0;
		myFlatColors[vertice + 14] = 1;
		myFlatColors[vertice + 15] = 1;

		

		myFlatColors[vertice + 16] = 0;
		myFlatColors[vertice + 17] = 0;
		myFlatColors[vertice + 18] = 1;
		myFlatColors[vertice + 19] = 1;


		myFlatColors[vertice + 20] = 0;
		myFlatColors[vertice + 21] = 0;
		myFlatColors[vertice + 22] = 1;
		myFlatColors[vertice + 23] = 1;
		vertice += 24;

		myFlatTexture[texVertice] = 1.0f;
		myFlatTexture[texVertice + 1] = 0.0f;

		myFlatTexture[texVertice +2] = 0.0f;
		myFlatTexture[texVertice + 3] = 1.0f;

		myFlatTexture[texVertice +4] = 0.0f;
		myFlatTexture[texVertice + 5] = 0.0f;

		myFlatTexture[texVertice +6] = 1.0f;
		myFlatTexture[texVertice +7] = 0.0f;

		myFlatTexture[texVertice+8] = 1.0f;
		myFlatTexture[texVertice + 9] = 1.0f;

		myFlatTexture[texVertice +10] = 0.0f;
		myFlatTexture[texVertice + 11] = 1.0f;

		texVertice += 12;

	}

	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myFlat); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myFlatTexture); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 288);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));

//	spColored->use(); //Aktywuj program cieniujący
//	glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
//	glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
//	glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
//
//
//	glEnableVertexAttribArray(spColored->a("vertex"));
//	glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, myFlat); //Współrzędne wierzchołków bierz z tablicy birdVertices
//
//a	glEnableVertexAttribArray(spColored->a("color"));
//	glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, myFlatColors); //Współrzędne wierzchołków bierz z tablicy birdColors
//
//	glDrawArrays(GL_TRIANGLES, 0, 1440);
//
//	glDisableVertexAttribArray(spColored->a("vertex"));
//	glDisableVertexAttribArray(spColored->a("color"));


};

void drawFlag(glm::mat4 P, glm::mat4 V, glm::mat4 M, float angle) {

	int const count = 1440;
	float myFlat[count];
	int vertice = 0;
	float myFlatColors[count];
	float myFlatTexture[count / 2];
	int texVertice = 0;

	for (float i = -30; i < 30; i = i + 1.0f)
	{
		// pierwszy trójkąt
			

		myFlat[vertice] = i + 1.0f;
		myFlat[vertice + 1] = 0.5* cos(water_speed * (time + (30 + i + 1.0f)));
		myFlat[vertice + 2] = -2;
		myFlat[vertice + 3] = 1;

		myFlat[vertice + 4] = i;
		myFlat[vertice + 5] = 0.5* cos(water_speed * (time + (30 + i)));
		myFlat[vertice + 6] = 2;
		myFlat[vertice + 7] = 1;

		myFlat[vertice + 8] = i;
		myFlat[vertice + 9] = 0.5* cos(water_speed * (time + (30 + i)));
		myFlat[vertice + 10] = -2;
		myFlat[vertice + 11] = 1;

		// drugi trójkąt


		myFlat[vertice + 12] = i + 1.0f;
		myFlat[vertice + 13] = 0.5* cos(water_speed * (time + (30 + i + 1.0f)));
		myFlat[vertice + 14] = -2;
		myFlat[vertice + 15] = 1;

		myFlat[vertice + 16] = i + 1.0f;
		myFlat[vertice + 17] = 0.5* cos(water_speed * (time + (30 + i + 1.0f)));;
		myFlat[vertice + 18] = 2;
		myFlat[vertice + 19] = 1;


		myFlat[vertice + 20] = i;
		myFlat[vertice + 21] = 0.5* cos(water_speed * (time + (30 + i)));
		myFlat[vertice + 22] = 2;
		myFlat[vertice + 23] = 1;


		myFlatTexture[texVertice] = 0;


		myFlatColors[vertice] = 0;
		myFlatColors[vertice + 1] = 0;
		myFlatColors[vertice + 2] = 1;
		myFlatColors[vertice + 3] = 1;


		myFlatColors[vertice + 4] = 0;
		myFlatColors[vertice + 5] = 0;
		myFlatColors[vertice + 6] = 1;
		myFlatColors[vertice + 7] = 1;



		myFlatColors[vertice + 8] = 0;
		myFlatColors[vertice + 9] = 0;
		myFlatColors[vertice + 10] = 1;
		myFlatColors[vertice + 11] = 1;



		myFlatColors[vertice + 12] = 0;
		myFlatColors[vertice + 13] = 0;
		myFlatColors[vertice + 14] = 1;
		myFlatColors[vertice + 15] = 1;



		myFlatColors[vertice + 16] = 0;
		myFlatColors[vertice + 17] = 0;
		myFlatColors[vertice + 18] = 1;
		myFlatColors[vertice + 19] = 1;


		myFlatColors[vertice + 20] = 0;
		myFlatColors[vertice + 21] = 0;
		myFlatColors[vertice + 22] = 1;
		myFlatColors[vertice + 23] = 1;
		vertice += 24;

		myFlatTexture[texVertice] = 1.0f;
		myFlatTexture[texVertice + 1] = 0.0f;

		myFlatTexture[texVertice + 2] = 0.0f;
		myFlatTexture[texVertice + 3] = 1.0f;

		myFlatTexture[texVertice + 4] = 0.0f;
		myFlatTexture[texVertice + 5] = 0.0f;

		myFlatTexture[texVertice + 6] = 1.0f;
		myFlatTexture[texVertice + 7] = 0.0f;

		myFlatTexture[texVertice + 8] = 1.0f;
		myFlatTexture[texVertice + 9] = 1.0f;

		myFlatTexture[texVertice + 10] = 0.0f;
		myFlatTexture[texVertice + 11] = 1.0f;

		texVertice += 12;

	}

	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myFlat); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myFlatTexture); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FlagTex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 288);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
};

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y, float bridge_angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania



	
	


	
	glm::mat4 M2 = glm::mat4(1.0f);
	M2 = glm::translate(M, glm::vec3(0.0f, -1.0f, -0.7f));
	M = glm::mat4(1.0f);
	M = glm::rotate(M2, bridge_angle *PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
	M = glm::translate(M, glm::vec3(0.0f, -2.0f, 0.0f));


	
	drawBridge(P, V, M);


	M = glm::mat4(1.0f);
	M = glm::rotate(M2, 90 *PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
	M = glm::scale(M, glm::vec3(1.0f, 0.59f, 1.0f));
	M = glm::translate(M, glm::vec3(0.0f, -8.7f, 0.0f));

	drawBridge(P, V, M);

	for (float i = -2; i <51; i = i + 8)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(7.0f, -1.91f, i-1.0f));
		M = glm::scale(M, glm::vec3(1.4f, 1.0f, 2.0f));

		drawWater(P, V, M, angle_x);
	}

	
	
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(19, 18, -4));
		M = glm::scale(M, glm::vec3(0.3f, 10.0f, 0.1f));
		drawFlag(P, V, M, angle_x);



	for (float i = -61; i < 60; i =i+2)
	{

		for (float j = -62; j < 60; j = j+2)
		{
			if ((i <-32 || i >31))
			{
				M = glm::mat4(1.0f);
				M = glm::translate(M, glm::vec3(i, -2.0f, j));
				drawGround(P, V, M);
			}
			else
			{
				if ((j < -6 || j >48))
				{
					M = glm::mat4(1.0f);
					M = glm::translate(M, glm::vec3(i, -2.0f, j));
					drawGround(P, V, M);
				}
			}
			
		}
		

	}


	//SAND
	for (float i = -17; i < 18; i = i+2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 0.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 42.0f));
		drawSand(P, V, M);
	
	}
	for (float i = -31; i < 32; i = i + 2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, -6.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 50.0f));
		drawSand(P, V, M);
	}

	for (float i = -29; i < -21; i = i + 2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 8.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 34.0f));
		drawSand(P, V, M);
	}

	for (float i = 23; i < 31; i = i + 2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 8.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 34.0f));
		drawSand(P, V, M);
	}
	for (float i = -29; i < -17; i = i + 2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, -4.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 46.0f));
		drawSand(P, V, M);

	/*	M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 50.0f));
		drawSand(P, V, M);*/
	}
	for (float i = 19; i < 31; i = i + 2)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, -4.0f));
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::translate(M, glm::vec3(i, -2.0f, 46.0f));
		drawSand(P, V, M);

		/*	M = glm::mat4(1.0f);
			M = glm::translate(M, glm::vec3(i, -2.0f, 50.0f));
			drawSand(P, V, M);*/
	}
	for (float i = 5.3; i >-50; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 33.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -31.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}
	for (float i = 3.3; i > -3; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 19.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -17.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}
	for (float i = 3.3; i > -7; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 31.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -29.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}
	for (float i = -34.7; i > -46; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 31.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -29.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}
	for (float i = -38.7; i > -46; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 19.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -17.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}

	for (float i = -1.3; i > -42; i = i - 2)
	{
		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, 27.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);

		M = glm::mat4(1.0f);
		M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		M = glm::translate(M, glm::vec3(i, -1.0f, -25.0f));
		//M = glm::rotate(M2, 90 * PI / 180.0f, glm::vec3(0.0f, 0.1f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
		drawSand(P, V, M);
	}
	//ENDSAND

	/*M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(1.0f, -2.0f, 0.0f));
	drawSand(P, V, M);
	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(-1.0f,-2.0f, 0.0f));
	drawSand(P, V, M);*/



	//kostka(P, V, M);

	//Podłoga zamek
	for (float i = -25; i < 26; i = i + 2)
	{
		for (float j = 40; j >= 4; j = j - 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, -2.0f, j));
			texCastleGround(P, V, M);

		}
	}
	for (float i = -3; i < 3; i = i + 2)
	{
		for (float j = 4; j >= 0 ; j = j - 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, -2.0f, j));
			texCastleGround(P, V, M);

		}
	}


	//MUR

	//SCIANA TYŁ
	for (float i = -25; i < 26; i = i + 2)
	{
		for (float j = 36; j <= 40; j = j + 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 0.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 2.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 4.0f, j));
			texKostka(P, V, M);
		}
	}

	//SCIANA PRAWY PRZÓD
	for (float i = -3; i > -26; i = i - 2)
	{
		for (float j = 0; j <= 4; j = j + 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 0.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 2.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 4.0f, j));
			texKostka(P, V, M);
		}

	}
	//SCIANA LEWY PRZÓD DŁ = 24
	for (float i = 3; i < 26; i = i + 2)
	{
		for (float j = 0; j <= 4; j = j + 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 0.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 2.0f, j));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(i, 4.0f, j));
			texKostka(P, V, M);
		}
	}

	//SCIANA LEWA
	for (float i = 6; i < 41; i = i + 2)
	{
		for (float j = 21; j <= 25; j = j + 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 0.0f, i));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 2.0f, i));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 4.0f, i));
			texKostka(P, V, M);
		}
	}

	//SCIANA PRAWA
	for (float i = 6; i < 41; i = i + 2)
	{
		for (float j = -21; j >= -25; j = j - 2)
		{
			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 0.0f, i));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 2.0f, i));
			texKostka(P, V, M);

			M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
			M = glm::translate(M, glm::vec3(j, 4.0f, i));
			texKostka(P, V, M);
		}
	}
	//WIEŻA LEWY PRZÓD
	for (float i = 19; i <= 29; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 0.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 2.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 4.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 8.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 10.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 12.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 14.0f, -4));
		texKostka(P, V, M);

	}

	for (float j = -2; j <= 6; j = j + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 0.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 2.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 4.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 6.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 8.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 10.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 12.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 14.0f, j));
		texKostka(P, V, M);


	}
	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(27, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, -2));
		texKostka(P, V, M);
	}

	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(21, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 4));
		texKostka(P, V, M);
	}

	//WIEŻA PRAWY PRZÓD
	for (float i = -29; i <= -19; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 0.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 2.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 4.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 8.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 10.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 12.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 14.0f, -4));
		texKostka(P, V, M);

	}

	for (float j = -4; j <= 6; j = j + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 0.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 2.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 4.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 6.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 8.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 10.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 12.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 14.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, -2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-27, i, 6));
		texKostka(P, V, M);
	}

	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-21, i, 6));
		texKostka(P, V, M);

	}
	//WIEŻA PRAWY TYŁ
	for (float i = -29; i <= -19; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 0.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 2.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 4.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 8.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 10.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 12.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 14.0f, 44));
		texKostka(P, V, M);
	}

	for (float j = 34; j <= 44; j = j + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 0.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 2.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 4.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 6.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 8.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 10.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 12.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 14.0f, j));
		texKostka(P, V, M);


	}
	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-27, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 42));
		texKostka(P, V, M);
	}

	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-21, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 36));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 34));
		texKostka(P, V, M);
	}

	//WIEŻA LEWY TYŁ
	for (float i = 19; i <= 29; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 0.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 2.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 4.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 8.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 10.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 12.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 14.0f, 44));
		texKostka(P, V, M);
	}

	for (float j = 34; j <= 44; j = j + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 0.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 2.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 4.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 6.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 8.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 10.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 12.0f, j));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 14.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(27, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 42));
		texKostka(P, V, M);
	}

	for (int i = 0; i <= 14; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 42));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 36));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(21, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 34));
		texKostka(P, V, M);
		
	}

	//Zdobienia Muru
	for (float i = 3; i < 26; i = i + 4)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, 0));
		texKostka(P, V, M);
	}

	for (float i = -3; i > -25; i = i - 4)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, 0));
		texKostka(P, V, M);
	}

	for (float i = 6; i < 41; i = i + 4)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, 6.0f, i));
			texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, 6.0f, i));
		texKostka(P, V, M);
	}

	for (float i = -25; i < 26; i = i + 4)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 6.0f, 40));
		texKostka(P, V, M);
	}
	//Zdobienia Wież

		//WIEŻA LEWY PRZÓD
	
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, 16.0f, 6));
		texKostka(P, V, M);

		//WIEŻA PRAWY PRZÓD

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 0));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, 16.0f, -4));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, 16.0f, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, 16.0f, 6));
		texKostka(P, V, M);

		//WIEŻA PRAWY TYŁ

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-23, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-25, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, 16.0f, 34));
		texKostka(P, V, M);

		//WIEŻA LEWY TYŁ

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 40));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 38));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, 16.0f, 44));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(25, 16.0f, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(23, 16.0f, 34));
		texKostka(P, V, M);

	//for (float i = -26; i < 27; i= i+4)
	//{
	//	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	//	M = glm::translate(M, glm::vec3(i, 6.0f, 0.0f));
	//	texKostka(P, V, M);
	//}




	//for (float i = -3; i > -26; i = i -4)
	//{
	//	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	//	M = glm::translate(M, glm::vec3(i, 6.0f, 0.0f));
	//	texKostka(P, V, M);
	//}



	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(1.0f, 4.0f, 0.0f));
	texKostka(P, V, M);
	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(-1.0f, 4.0f, 0.0f));
	texKostka(P, V, M);

	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(1.0f, 4.0f, 2.0f));
	texKostka(P, V, M);
	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(-1.0f, 4.0f, 2.0f));
	texKostka(P, V, M);

	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(1.0f, 4.0f, 4.0f));
	texKostka(P, V, M);
	M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(-1.0f, 4.0f, 4.0f));
	texKostka(P, V, M);


	

	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	//Główna pętla
	float angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float bridge_angle = 180;
	glfwSetTime(0); //Wyzeruj licznik czasu


	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		if (getBridgeDown)
		{
			bridge_angle += bridgeSpeed * glfwGetTime();
			if (bridge_angle <90)
			{
				bridge_angle = 90;
			}
		}
		else
		{
			bridge_angle += bridgeSpeed * glfwGetTime();
			if (bridge_angle >180)
			{
				bridge_angle = 180;
			}
		}

		angle_x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		time += glfwGetTime();
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, angle_x, angle_y,bridge_angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}