#include "MovmentControll.h"

glm::vec2 MovmentControll::acc = { 0, 0 };
glm::vec2 MovmentControll::pos_last = { 0, 0 };
glm::vec2 MovmentControll::move = { 0,0 };
glm::vec2 MovmentControll::screen_size = { 0, 0 };
glm::vec2 MovmentControll::actual = { 0,0 };
float MovmentControll::low_barier = 1.5f;
float MovmentControll::high_barier = 20.0f;
float MovmentControll::drag = 0.75;
float MovmentControll::zoom = 1.0f;
float MovmentControll::zoom_acc = 0.0f;
float MovmentControll::low_hard_barier = 1.0f;
float MovmentControll::high_hard_barier = 25.0f;

bool MovmentControll::isMiddlePressed = false;
