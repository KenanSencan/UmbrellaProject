#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>

int main() {
    if (!glfwInit())
        return -1;

    // Enable transparent framebuffer
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Remove window border
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);   // Always on top
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Transparent Pet", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Random rectangle position and size
    float rectX = (std::rand() % 600) / 800.0f * 2.0f - 1.0f;
    float rectY = (std::rand() % 400) / 600.0f * 2.0f - 1.0f;
    float rectSize = 0.2f;

    while (!glfwWindowShouldClose(window)) {
        // Clear with full transparency (alpha = 0)
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw rectangle with solid color
        glBegin(GL_QUADS);
        glColor4f(1.0f, 0.0f, 0.5f, 1.0f); // Pink, fully opaque
        glVertex2f(rectX, rectY);
        glVertex2f(rectX + rectSize, rectY);
        glVertex2f(rectX + rectSize, rectY + rectSize);
        glVertex2f(rectX, rectY + rectSize);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}