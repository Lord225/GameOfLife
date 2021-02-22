#include "MovmentControll.h"

glm::vec2 MovmentControll::acc = { 0, 0 };
glm::vec2 MovmentControll::pos_last = { 0, 0 };
glm::vec2 MovmentControll::move = { 0,0 };
glm::vec2 MovmentControll::screen_size = { 0, 0 };
glm::vec2 MovmentControll::actual = { 0,0 };

bool MovmentControll::isMiddlePressed = false;
float MovmentControll::zoom = 1.0f;
float MovmentControll::zoom_acc = 0.0f;

// Setting for camera behavior.
float MovmentControll::low_barier = 1.5f;    //Maximal zoom
float MovmentControll::high_barier = 20.0f;  //Minimal zoom
float MovmentControll::drag = 0.75;          //Drag for mouse and scroll deacceleration     

float MovmentControll::low_hard_barier = 1.0f;   //unused
float MovmentControll::high_hard_barier = 25.0f; //unused


