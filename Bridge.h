#ifndef BRIDGE_H_INCLUDED
#define BRIDGE_H_INCLUDED

int myBridgeVertexCount = 36;

float myBridgeVertices[] = {
	//�ciana 1 - czo�o
	-2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,

2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

//�ciana 2 - ty�(czo�o)
	-2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,

2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,


//�ciana 3 - lewa
	-2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
-2.0f, 2.0f, 0.3f, 1.0f,

-2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,

//�ciana 4 - prawa
	2.0f, 2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

2.0f, -2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,

//�ciana 5 - d�
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,

-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

//�ciana 6 - g�ra
-2.0f, 2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, 2.0f, 0.3f, 1.0f,

-2.0f, 2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,



};

#endif // MYWATER_H_INCLUDED