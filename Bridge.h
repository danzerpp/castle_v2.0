#ifndef BRIDGE_H_INCLUDED
#define BRIDGE_H_INCLUDED

int myBridgeVertexCount = 36;

float myBridgeVertices[] = {
	//�ciana 1 - czo�o
	-1.9f, 1.9f, 0.0f, 1.0f,
-1.9f, -1.9f, 0.0f, 1.0f,
1.9f, 1.9f, 0.0f, 1.0f,

1.9f, 1.9f, 0.0f, 1.0f,
-1.9f, -1.9f, 0.0f, 1.0f,
1.9f, -1.9f, 0.0f, 1.0f,

//�ciana 2 - ty�(czo�o)
	-1.9f, 1.9f, 0.8f, 1.0f,
-1.9f, -1.9f, 0.8f, 1.0f,
1.9f, 1.9f, 0.8f, 1.0f,

1.9f, 1.9f, 0.8f, 1.0f,
-1.9f, -1.9f, 0.8f, 1.0f,
1.9f, -1.9f, 0.8f, 1.0f,


//�ciana 3 - lewa
	-1.9f, 1.9f, 0.0f, 1.0f,
-1.9f, -1.9f, 0.0f, 1.0f,
-1.9f, 1.9f, 0.8f, 1.0f,

-1.9f, 1.9f, 0.8f, 1.0f,
-1.9f, -1.9f, 0.8f, 1.0f,
-1.9f, -1.9f, 0.0f, 1.0f,

//�ciana 4 - prawa
	1.9f, 1.9f, 0.0f, 1.0f,
1.9f, 1.9f, 0.8f, 1.0f,
1.9f, -1.9f, 0.0f, 1.0f,

1.9f, -1.9f, 0.0f, 1.0f,
1.9f, 1.9f, 0.8f, 1.0f,
1.9f, -1.9f, 0.8f, 1.0f,

//�ciana 5 - d�
-1.9f, -1.9f, 0.0f, 1.0f,
1.9f, -1.9f, 0.0f, 1.0f,
-1.9f, -1.9f, 0.8f, 1.0f,

-1.9f, -1.9f, 0.8f, 1.0f,
1.9f, -1.9f, 0.8f, 1.0f,
1.9f, -1.9f, 0.0f, 1.0f,

//�ciana 6 - g�ra
-1.9f, 1.9f, 0.0f, 1.0f,
1.9f, 1.9f, 0.0f, 1.0f,
-1.9f, 1.9f, 0.8f, 1.0f,

-1.9f, 1.9f, 0.8f, 1.0f,
1.9f, 1.9f, 0.8f, 1.0f,
1.9f, 1.9f, 0.0f, 1.0f,



};

#endif // MYWATER_H_INCLUDED