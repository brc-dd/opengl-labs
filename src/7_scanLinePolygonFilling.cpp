#include "pch.h"

constexpr unsigned MAX_PX = 1024;

int le[MAX_PX], re[MAX_PX];

void putPixel(int x, int y) {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glEnable(GL_SCISSOR_TEST);
    glScissor(x + vp[2] / 2, y + vp[3] / 2, 1, 1);
    glClearColor(0.00f, 0.57f, 0.92f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void edge(int x0, int y0, int x1, int y1) {
    if (y1 < y0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int x = x0;
    int m = (y1 - y0) / (x1 - x0);
    for (int i = y0; i < y1; i++) {
        if (x < le[i]) le[i] = x;
        if (x > re[i]) re[i] = x;
        x += (1 / m);
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
    auto window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ScanLine Polygon Filling", nullptr, nullptr);

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

    // build and compile our shader program
    // ------------------------------------
    Shader shader("1_rectangle.vert", "1_rectangle.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.0f,  0.67f,  0.0f, // top
        0.5f,  0.0f,   0.0f, // right
        0.0f,  -0.67f, 0.0f, // bottom
        -0.5f, 0.0f,   0.0f  // left
    };

    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object
    // ----------------------------
    glBindVertexArray(VAO);

    // bind and set vertex buffer(s)
    // -----------------------------
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // configure vertex attributes(s)
    // ------------------------------
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    // -----------
    for (glfwSetTime(0.00); !glfwWindowShouldClose(window);) {

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.02f, 0.26f, 0.37f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a triangle
        // ---------------
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < MAX_PX; ++i) {
            le[i] = MAX_PX;
            re[i] = 0;
        }

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);

        edge(0.00f + vp[2], 0.67f * vp[3] / 2 + vp[2], 0.50f * vp[2] / 2 + vp[2], 0.00f + vp[2]);
        edge(0.50f * vp[2] / 2 + vp[2], 0.00f + vp[2], 0.00f + vp[2], -0.67f * vp[3] / 2 + vp[2]);
        edge(0.00f + vp[2], -0.67f * vp[3] / 2 + vp[2], -0.50f * vp[2] / 2 + vp[2], 0.00f + vp[2]);
        edge(-0.50f * vp[2] / 2 + vp[2], 0.00f + vp[2], 0.00f + vp[2], 0.67f * vp[3] / 2 + vp[2]);

        for (int i = 0; i < MAX_PX and i < glfwGetTime() * 100; ++i)
            for (int j = le[i]; j < re[i]; ++j) putPixel(j - vp[2], i - vp[2]);

        // glfw: swap buffers and poll IO events
        // -------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    // -------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    // -----------------------------------------------------------------
    glfwTerminate();
}
