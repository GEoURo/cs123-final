/**
 * @file   OrbitingCamera.cpp
 *
 * (See the header file.) You don't need to be poking around in this file unless you're interested
 * in how an orbiting camera works.
 *
 * The way we have implemented this class is NOT how you should be implementing your Camtrans. This
 * camera is a DIFFERENT TYPE of camera which we're providing so you can easily view your Shapes
 * and to make sure your scene graph is working if your camera isn't.
 *
 * In the Camtrans lab, you'll be implementing your own perspective camera from scratch! This one
 * uses OpenGL.
 */

#include <float.h>
#include <math.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "OrbitingCamera.h"
#include "utils/settings.h"

void OrbitingCamera::setAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
    updateProjectionMatrix();
}

glm::mat4x4 OrbitingCamera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

glm::mat4x4 OrbitingCamera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::mat4x4 OrbitingCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

void OrbitingCamera::mouseDown(int x, int y) {
    m_oldX = x;
    m_oldY = y;
}

void OrbitingCamera::mouseDragged(int x, int y) {
    m_angleY += x - m_oldX;
    m_angleX += y - m_oldY;
    m_oldX = x;
    m_oldY = y;
    if (m_angleX < -90) m_angleX = -90;
    if (m_angleX > 90) m_angleX = 90;

    updateViewMatrix();
}

void OrbitingCamera::mouseScrolled(int delta) {
    // Use an exponential factor so the zoom increments are small when we are
    // close to the object and large when we are far away from the object
    m_zoomZ *= powf(0.999f, delta);

    updateViewMatrix();
}

void OrbitingCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
//    glm::vec4 w = -glm::normalize(look);
//    glm::vec4 v = glm::normalize(up - glm::dot(up, w) * w);
//    glm::vec4 u = glm::vec4(glm::cross(glm::vec3(v), glm::vec3(w)), 0.f);

//    m_rotationMatrix = glm::transpose(glm::mat4(u, v, w, glm::vec4(0.f, 0.f, 0.f, 1.f)));

//    m_translationMatrix = mat4(1.f);
//    m_translationMatrix[3] = vec4(-eye.x, -eye.y, -eye.z, 1.f);

//        updateViewMatrix();
}

void OrbitingCamera::updateMatrices() {
    updateProjectionMatrix();
    updateViewMatrix();
}

void OrbitingCamera::updateProjectionMatrix() {
    // FIXME: settings from UI
    float fov = 55;settings.cameraFov;
    // Make sure glm gets a far value that is greater than the near value.
    float nearPlane = 0.1f;settings.cameraNear;
    float farPlane = std::max(50.f, nearPlane + 100.f * FLT_EPSILON);
    float h = farPlane * glm::tan(glm::radians(fov * 0.5f));
    float w = m_aspectRatio * h;

    m_scaleMatrix = glm::scale(glm::vec3(1.f / w, 1.f / h, 1.f / farPlane));
    m_projectionMatrix = glm::perspective(glm::radians(fov), m_aspectRatio, nearPlane, farPlane) * 0.02f;
}

void OrbitingCamera::updateViewMatrix() {
    m_viewMatrix =
            glm::translate(glm::vec3(0.f, 0.f, m_zoomZ)) *
            glm::rotate(glm::radians(m_angleY), glm::vec3(0.f, 1.f, 0.f)) *
            glm::rotate(glm::radians(m_angleX), glm::vec3(1.f, 0.f, 0.f));
}
