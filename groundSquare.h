#ifndef GROUNDSQUARE_H_INCLUDED
#define GROUNDSQUARE_H_INCLUDED

int myGroundSquareVertexCount = 6;

float myGroundSquareVertices[] = {

-5.0f, 5.0f, 5.0f, 5.0f,
5.0f, 5.0f, -5.0f, 5.0f,
5.0f, 5.0f, 5.0f, 5.0f,

-5.0f, 5.0f, 5.0f, 5.0f,
-5.0f, 5.0f, -5.0f, 5.0f,
5.0f, 5.0f, -5.0f, 5.0f,

};


float myGroundSquareTexCoords[] = {
		1.0f, 0.0f,	  0.0f, 1.0f,    0.0f, 0.0f,
		1.0f, 0.0f,   1.0f, 1.0f,    0.0f, 1.0f,

};

#endif // SQUARE_H_INCLUDED