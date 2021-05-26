
#include <glm/glm.hpp>


struct Layout {
    glm::mat4 projection;
    glm::mat4 transform;

    Layout(glm::vec2& WindowResolution, glm::vec2& ViewResolution){
        float scaleFactor = min(WindowResolution.x / ViewResolution.x, WindowResolution.y / ViewResolution.y);

        glm::vec2 effectiveSize = ViewResoltion * scaleFactor;
        glm::vec2 offset = (WindowResolution / 2.0f) - (effectiveSize / 2.0f);

        transform = glm::mat4(1.0f);
        transform = glm::scale(transform,glm::vec3{scaleFactor,scaleFactor,0});
        transform = glm::translate()
    }
}

struct LayoutManager {
    const glm::vec2 baseSize;

    static glm::vec2 canvasOffset;
    static float scaleFactor;

    static Calculate()
}
