#ifndef OBJECTS_H
#define OBJECTS_H

// 도로 그리기
void drawRoad();

// 가로수 그리기
void drawTreeTrunk(float x);  // 가로수 줄기
void drawTreeLeaves(float x); // 가로수 잎

// 가로등 그리기
void drawLampPost(float x);   // 가로등 기둥
void drawLampLight(float x);  // 가로등 등불

//구름 그리기
void drawCloud(float x, float y);
#endif // OBJECTS_H