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
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

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

glm::vec4 OrbitingCamera::getPosition() const {
    return m_viewMatrixInverse * glm::vec4(0, 0, 0, 1);
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
    m_translateZ *= powf(0.999f, delta);

    updateViewMatrix();
}

void OrbitingCamera::reset() {
    orientLook(glm::vec4(5, 5, 5, 1), glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 1, 0));
}

void OrbitingCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_look = look;
    m_up = up;

    m_w = -glm::normalize(look);
    m_v = glm::normalize(up - glm::dot(up, m_w) * m_w);
    m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0.f);

    m_originalView = glm::lookAt(glm::vec3(eye), glm::vec3(eye + look), glm::vec3(up));

    m_translateX = m_translateZ = m_angleX = m_angleY = 0.f;

    updateViewMatrix();
    updateProjectionMatrix();
}

void OrbitingCamera::updateMatrices() {
    updateProjectionMatrix();
}

void OrbitingCamera::updateProjectionMatrix() {
    float fov = settings.cameraFov;
    // Make sure glm gets a far value that is greater than the near value.
    float nearPlane = settings.cameraNear;
    float farPlane = std::max(settings.cameraFar, nearPlane + 100.f * FLT_EPSILON);
    float h = farPlane * glm::tan(glm::radians(fov * 0.5f));
    float w = m_aspectRatio * h;

    m_scaleMatrix = glm::scale(glm::vec3(1.f / w, 1.f / h, 1.f / farPlane));
    m_projectionMatrix = glm::perspective(glm::radians(fov), m_aspectRatio, nearPlane, farPlane) * 0.02f;
}

void OrbitingCamera::updateViewMatrix() {
    m_viewMatrix = glm::translate(vec3(m_translateX, 0, m_translateZ)) *
            m_originalView *
                   glm::rotate(radians(m_angleY), vec3(0.f, 1.f, 0.f)) *
                   glm::rotate(radians(m_angleX), vec3(1.f, 0.f, 0.f));

    m_viewMatrixInverse = glm::inverse(m_viewMatrix);
}

void OrbitingCamera::moveForward() {
    m_translateZ += 0.1;
    updateViewMatrix();
}

void OrbitingCamera::moveBackward() {
    m_translateZ -= 0.1;
    updateViewMatrix();
}

void OrbitingCamera::moveRight() {
    m_translateX -= 0.1;
    updateViewMatrix();
}

void OrbitingCamera::moveLeft() {
    m_translateX += 0.1;
    updateViewMatrix();
}
