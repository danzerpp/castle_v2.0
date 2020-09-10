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
#include "Bridge.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]

GLuint tex;
float yaw = 90;
float pitch = 0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400, lastY = 300;
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

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
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
	float groundVertices[] = {
		100,-1,100,1,
		100,-1,-100,1,
		-100,-1,100,1,

		-100,-1,100,1,
		-100,-1,-100,1,
		100,-1,-100,1,
	};

	float groundColors[] = {
		0,1,0,1,
		0,1,0,1,
		0,1,0,1,

		0,1,0,1,
		0,1,0,1,
		0,1,0,1,
	};

	spColored->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spColored->a("vertex"));
	glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, groundVertices); //Współrzędne wierzchołków bierz z tablicy birdVertices

	glEnableVertexAttribArray(spColored->a("color"));
	glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, groundColors); //Współrzędne wierzchołków bierz z tablicy birdColors

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(spColored->a("vertex"));
	glDisableVertexAttribArray(spColored->a("color"));

	//spLambert->use(); //Aktyeuj program cieniujący
	//glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	//glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	//glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu

	// M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	//M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
	//Models::torus.drawSolid(); //Narysuj obiekt

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
	spColored->use(); //Aktywuj program cieniujący
	M = glm::rotate(M, 44.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(0.0f, 1.0f, -1.0f));
	glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spColored->a("vertex"));
	glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, myBridgeVertices); //Współrzędne wierzchołków bierz z tablicy birdVertices

	glEnableVertexAttribArray(spColored->a("color"));
	glVertexAttribPointer(spColored->a("color"), 4, GL_FLOAT, false, 0, myCubeColors); //Współrzędne wierzchołków bierz z tablicy birdColors

	glDrawArrays(GL_TRIANGLES, 0, myBridgeVertexCount);

	glDisableVertexAttribArray(spColored->a("vertex"));
	glDisableVertexAttribArray(spColored->a("color"));
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

void texKostka2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
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

	float myCubeNormals[] = {
		0,0,-1,0, 0,0,-1,0, 0,0,-1,0,
		0,0,-1,0, 0,0,-1,0, 0,0,-1,0,

		0,0, 1,0, 0,0, 1,0, 0,0, 1,0,
		0,0, 1,0, 0,0, 1,0, 0,0, 1,0,

		0,-1,0,0, 0,-1,0,0, 0,-1,0,0,
		0,-1,0,0, 0,-1,0,0, 0,-1,0,0,

		0, 1,0,0, 0, 1,0,0, 0, 1,0,0,
		0, 1,0,0, 0, 1,0,0, 0, 1,0,0,

		-1,0,0,0, -1,0,0,0, -1,0,0,0,
		-1,0,0,0, -1,0,0,0, -1,0,0,0,

		1,0,0,0, 1,0,0,0, 1,0,0,0,
		1,0,0,0, 1,0,0,0, 1,0,0,0,
	};

	spLambertTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals); //Wektory normalne bierz z tablicy myCubeNormals

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("color"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::rotate(M, angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M = glm::rotate(M, angle_x, glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania



	drawGround(P, V, M);
	M = glm::mat4(1.0f);
	drawBridge(P, V, M);

	//kostka(P, V, M);

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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 16.0f, -4));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 16; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(27, i, 6));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, -2));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 16.0f, -4));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-27, 16.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 16; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, -2));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-27, i, 6));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 16.0f, 44));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-29, 16.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 16; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-27, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(-19, i, 42));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(i, 16.0f, 44));
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

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(29, 16.0f, j));
		texKostka(P, V, M);

	}
	for (int i = 0; i <= 16; i = i + 2)
	{
		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(27, i, 34));
		texKostka(P, V, M);

		M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M = glm::translate(M, glm::vec3(19, i, 42));
		texKostka(P, V, M);
	}

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



	//texKostka2(P, V, M);

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
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}