#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

int mySquareVertexCount = 6;

float mySquareVertices[] = {

5.0f, -5.0f, -5.0f, 5.0f,
-5.0f, 5.0f, -5.0f, 5.0f,
-5.0f, -5.0f, -5.0f, 5.0f,

5.0f, -5.0f, -5.0f, 5.0f,
5.0f, 5.0f, -5.0f, 5.0f,
-5.0f, 5.0f, -5.0f, 5.0f,

};


float mySquareTexCoords[] = {
		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

};

#endif // SQUARE_H_INCLUDED