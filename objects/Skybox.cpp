#include "../textures/stb_image.h"

#include "Skybox.h"

Skybox::Skybox() {
    cube = std::make_shared<Mesh>(Mesh::cube());
    shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    cube->useShader(shader);

    glGenTextures(1, &cubeMap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    std::vector<std::string> textures_faces{
            "textures/Skybox_Water222_right.jpg",
            "textures/Skybox_Water222_left.jpg",
            "textures/Skybox_Water222_top.jpg",
            "textures/Skybox_Water222_base.jpg",
            "textures/Skybox_Water222_front.jpg",
            "textures/Skybox_Water222_back.jpg"
    };
    for (GLuint i = 0; i < textures_faces.size(); i++) {
        int width, height, nrChannels;
        auto data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }

    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Use bilinear interpolation:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Use clamp to edge to hide skybox edges:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &cubeMap);
}

void Skybox::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    // Use cube map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    // Inside the cube
    glCullFace(GL_FRONT);
    shader->use();
    shader->setUniform1i("cubemap", 0);
    auto view_origin = glm::mat4(glm::mat3(view)); // remove translation component
    cube->draw(world, projection, view_origin, eye);
    glCullFace(GL_BACK);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
