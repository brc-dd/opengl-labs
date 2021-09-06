#include "pch.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

std::filesystem::path getDirectory() {

    std::filesystem::path dir = ".";
    int length = wai_getExecutablePath(nullptr, 0, nullptr), dirname_length;

    if (length > 0) {
        auto path = new char[length + 1];
        if (!path) abort();

        wai_getExecutablePath(path, length, &dirname_length);
        path[dirname_length] = '\0';

        dir = path;
        delete path;
    }

    return dir;
}
