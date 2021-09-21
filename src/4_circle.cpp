#include "pch.h"

void putPixel(int x, int y) {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glEnable(GL_SCISSOR_TEST);
    glScissor(x + vp[2] / 2, y + vp[3] / 2, 1, 1);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void midPointCircleAlgo(int r) {
    int x = 0, y = r;

    // first point
    putPixel(x, y);

    // decision parameter
    float P = 5 / 4 - r;

    while (x < y) {
        P += 2 * (++x - (P < 0 ? 0 : --y)) + 1;

        for (int i = 0; i < 2; ++i)
            for (int j = -x; j <= x; j += 2 * x)
                for (int k = -y; k <= y; k += 2 * y) putPixel(i ? j : k, i ? k : j);
    }
}

int main() {
    // settings
    // --------
    const auto SCR_WIDTH = 800u;
    const auto SCR_HEIGHT = 600u;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: create window
    // -------------------
    auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rectangle", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.02f, 0.26f, 0.37f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a circle
        // -------------
        midPointCircleAlgo(150);

        // glfw: swap buffers and poll IO events
        // -------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources
    // -----------------------------------------------------------------
    glfwTerminate();
}
