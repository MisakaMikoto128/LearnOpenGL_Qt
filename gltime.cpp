#include "gltime.h"


long start = 0;

/*
This function returns the value of the GLFW timer.
Unless the timer has been set using glfwSetTime, the timer measures time elapsed since GLFW was initialized.
*/
double glfwGetTime(void)
{
    return double(clock() - start)/CLOCKS_PER_SEC;
}
