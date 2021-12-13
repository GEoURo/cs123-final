# Sceneviewer

### Introduction to Computer Graphics, Fall 2021

## Group

Salty Fish

Jianxin Gu  (jianxin_gu@brown.edu)
Shangqun Yu (shangqun_yu@brown.edu)
Zijian Chen (zijian_chen@brown.edu)

## Introduction

The sceneviewer is an extension for the Sceneview assignment and includes several advanced real-time graphic features.

1. More realistic texture mapping and lighting.
2. High Dynamic Range
3. Shadow mapping
4. Depth of Field

## Technical Features

### 1. Texture mapping

The original texture mapping calculation in the Sceneview is not quite correct and the lighting calculations are wrong, so we fix them and the texture mapping should look the same as the ray tracing results, which follows phong-illumination model and the texture component only blends with the diffuse component.

### 2. High Dynamic Range

The core idea of high dynamic range is to allow color data that are greater than 1 during calculation. And before the colors are sent to the display, we map them back to 0~1 using some functions. In this case, it would be a good idea to refactor our original OpenGL pipeline into one using FBO so that our other features would be just another FBO calculation stage, rather than modifying and clustering all code segments for each feature into one single shader.

With the modified FBO pipeline, the original phong-rendering pass still stays the same, except that its result is rendered into a FBO and stored. By configuring the data format of the color attachment of this FBO to `GL_FLOAT`, we can naturally have HDR support and store color data greater than 1. Then for the next FBO stage, we just take in the buffer with HDR data and apply tone mapping function to it.

### 3. Shadow Mapping

The shadow mapping also utilize FBO, with the only difference that the buffer it has is depth attachment. By rendering the scene from the light's perspective, we can have the depth data stored in the buffer. Then, when the phong-render pass happens, the depth buffer can be used to perform depth test to determine whether a fragment is in shadow or not.

Here are a few discussions on shadow mapping.

1. We consulted some forums and tutorials, and found that it is recommended that there shouldn't be more than 2 lights considered for shadow mapping if the naive appproch is used. Otherwise, it would be too computational expensive. If the number of light for shadow is too great, deferred-shading would be the preferred approach. Therefore, our sceneviewer only support one directional light.

2. By rendering the depth info into a 2D texture, we are only modeling the behavior of the directional light. For point light or omnidirectional shadow, cube texture should be used for the depth buffer. We planned to implement point shadow originally, and we did make a lot progress. However, we encountered a problem that is so 'stubborn' and counter-intuitive, which is unable to get any data into a FBO with a cube map as the depth map. The depth map will contain meaningless data even if we hard-code a fix value for every face with `gl_FragDepth = 0.1`. We even tried to copy-paste the demo code from 'LearnOpenGL' tutorial and it still refused to work. However, by building the demo project of 'LearnOpenGL', the point shadow code indeed worked. This means that the same code that works in the demo is somehow not working in our project. We spent a ton of time but still have not figured out why. In the end, we suspect it could be some overall environment issue with Qt and the project setup with the lack of any GPU debugging and profiling tools.

3. Although point shadow did not work, on our path to solve the problem, we did find an intutive way to demonstrate how the cube depth mapping is representing the depth info of the surroundings.

We made debug shaders that can visualize what the depth info is look like. It can be achieved with the following steps.

For point shadows
``` C++
1. Uncomment #define DEBUG_POINT_SHADOW in view.cpp
2. Change the line 396-399 of SceneviewScene.cpp as follow.

    // m_pointShadowDebugShader->setTexture("pointShadow.depthMap", m_pointShadowMap->getDepthCube());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, testCubeMap);
    glUniform1i(glGetUniformLocation(m_pointShadowDebugShader->getID(), "pointShadow.depthMap"), 0);

3. Open all_cheese.xml scene under the scenefiles directory of this project and check "Shadow mapping" in the UI.
```

For directional shadow
``` C++
1. Uncomment #define DEBUG_POINT_SHADOW in view.cpp

2. Open a scene file that contains directional light and check "Shadow mapping" in the UI.
```

The depth cube map used here is a pre-allocated cube texture with fixed values on each face. It shows that how the cube map is interpreting the surroundings. It also demonstrates that our method of sampling from the cube depth map is correct and the only problem is getting infomation into the cube texture inside the FBO.

Overall, we were planning to consider 2 shadows, one for directional light and one for point light. But due to the setback on cube texture with FBO, the point shadow did not work eventually. Therefore, there will be only one light in our scene can cast shadows. That is also why our scenefiles for demonstrating shadow mapping mostly only have one or two directional lights.

### 4. Depth of Field

For real-time graphics, the depth of field is achieved with some hacks. 

The overall idea is to blur the image accordingly by comparing its depth value with the focus. Thus, we need the depth info for the rendered scene. Luckily, this information is already there in the FBO for the phong-rendering pass. However, because it uses render buffer as the depth attachment, we can not sample from it in the later FBO passes. 

We resolved this by changing the type of the depth attachment to a texture. Then we just need to pass in both the depth map from the camera perspective and the rendered image as two textures into the fragment shader, and apply the blur accordingly to mimic the effect of DoF.

## Test scenes

All of our test scenes for demonstration can be found in the `scenefiles` directory of our repository.

## Words of thought

We are still very interested in finding out what on earth went wrong with our combination of FBO and cube textures. 

If you have any idea what is going on, please feel free to email as and discuss.