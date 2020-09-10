#ifndef BRIDGE_H_INCLUDED
#define BRIDGE_H_INCLUDED

int myBridgeVertexCount = 36;

float myBridgeVertices[] = {
	//œciana 1 - czo³o
	-2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,

2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

//œciana 2 - ty³(czo³o)
	-2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,

2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,


//œciana 3 - lewa
	-2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,
-2.0f, 2.0f, 0.3f, 1.0f,

-2.0f, 2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,
-2.0f, -2.0f, 0.0f, 1.0f,

//œciana 4 - prawa
	2.0f, 2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

2.0f, -2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,

//œciana 5 - dó³
-2.0f, -2.0f, 0.0f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,
-2.0f, -2.0f, 0.3f, 1.0f,

-2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.3f, 1.0f,
2.0f, -2.0f, 0.0f, 1.0f,

//œciana 6 - góra
-2.0f, 2.0f, 0.0f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,
-2.0f, 2.0f, 0.3f, 1.0f,

-2.0f, 2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.3f, 1.0f,
2.0f, 2.0f, 0.0f, 1.0f,



};

#endif // MYWATER_H_INCLUDED