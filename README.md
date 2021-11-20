# Final Project

### Introduction to Computer Graphics, Fall 2021


## Introduction

This fall, we’ve surveyed a number of topics in computer graphics to
give you a broad introduction to the field. Now, we’re giving you
the reins. Pick your favorite topic, do a little reading and research,
and make a cool demo. In the final project you will implement your
choice of rendering, modeling, or image-based techniques discussed
in class, which have not previously been covered in other homework
assignments, and show them off in an interesting scene. We provide
a list of suggestions, but you are free to choose what scene and
effects you want to implement, as long as you clear it with the TAs.
We will evaluate your project based on technical and creative merits.
You will submit all relevant parts of the final project using Google
Forms to preserve Gradescope anonymity.



## Groups

The final project is traditionally done in groups of 2-4 students with
the help of a mentor TA. You may work in a group by yourself, and
the requirements will be scaled down accordingly. However, we still
recommend working with a group if possible as you will be able to
combine different skill sets, talk through more difficult concepts and
divide the initial overhead of getting something to just work.

<b>Finding Teammates</b>. If you wish to work in a group and are
looking for teammates, we encourage you post in on the "[Finding final project groups](https://edstem.org/us/courses/12828/discussion/812015)" Ed thread with proposal ideas/features you are
interested in and form groups with other people who are match your
interests. Otherwise, you can also fill out [this](https://docs.google.com/forms/d/e/1FAIpQLSdA3AgdyY9TzdxqfcQjUM2lnHrs8RDrgrOu2aX4gRAzv_ZckQ/viewform?usp=sf_link) form, and we will do
our best to match you with other classmates with similar interests.
The deadline to submit this form is Monday, 11/15 at 11:59 p.m.
EST, and we will email you and your group members ASAP


## Requirements

<b>Project Proposals</b>. Your group must submit a project proposal
by Tuesday, 11/23 at 11:59 p.m. EST. This should include:

- Logins of all group members

- A description of the theme (Generally, what you expect your
final project to look like. Feel free to include multiple ideas.)

- Technical features you plan to implement. (Possible options for
features are listed below.)

- A public Github repo link (can be empty, a clone of the [stencil code](https://github.com/cs123tas/final-stencil) or using the [Github Classroom assignment](https://classroom.github.com/a/dNb58kXq)) If you want your Github repository to be private, you will need to add the HTAs as collaborators (and your mentor TA when you are assigned one). Submit your project proposal with the Google form [here](https://docs.google.com/forms/d/e/1FAIpQLSdZvFYnTE-iSV51DftaD_8joTQNNiA6j-7FCYTfP4CTZvmxIA/viewform?usp=sf_link).

<b>NOTE!</b> This project can be started using many diffent things from labs done throughout the semester, the stencil code, to a blank repository. The stencil code is there to help most real-time projects get a start however it does not have to be used.

<b>Mentor TAs</b>. Your group will be assigned a mentor TA shortly
after proposals are handed in. This TA will be your go-to resource
when working on your final project. If you have any questions, you
can direct your questions to them.

<b>Project Plans</b>. Your group must make a detailed plan for how
you will implement the technical features in your final project. This
should include:

- General description of how each feature will be implemented.
You may want to include pseudo-code for any complicated algorithms that will be involved. We just want to know that you have
a general understanding of how each feature is implemented.
- Resources that you will use while implementing each technical
feature. This may include SIGGRAPH papers, tutorials, or
anything else you find that will be helpful when learning about
the feature and how to implement it.
- High-level overview of the flow of your final program. How will
all the pieces fit together?
- Division of labor and plan of action. Who will do what? Where
will you start? This plan of action should account for the
possibility that you do not finish everything you set out to finish
(see the gear up about our expectations). Design your project
plan so that even if you only get part way through you still an
incomplete but displayable project.

The purpose of the final project plan is to ensure that each part
of your project is doable, and that you have a plan for how to
implement it before you start. To hand in the plan, just push the
plan document to Github repo.

<b>Design Checks</b>. You should email your mentor TA once you’ve
been assigned one to meet for a design check. Design checks should
take place by Tuesday, 11/30 at 11:59 p.m. EST. This will be an
opportunity for you to discuss your project plan with the TA and
get useful feedback.

<b>Project Status Updates</b>. By Thursday, 12/9 at 11:59 p.m. EST,
you should meet with your mentor TA to show your progress on the
final project. You must have some preliminary results to show at
this meeting.

## Technical Features

Overall, we want your final project to be cohesive and in-depth, so we
won’t have strict requirements for technical features to incorporate
into your project. As a general rule of thumb you should implement
about one new technical feature per group member, but if you want
to focus on one or two particular features (depending on the size of
your group) and implement it to a high degree of complexity that is
also great! Discuss with your mentor TA or one of the HTAs to see
whether your idea meets the expectations.
Here are some ideas for technical features you might implement.
However you can choose to do anything in the realm of graphics
(both 3D or 2D projects are acceptable). This [link](https://kesen.realtimerendering.com/) has compiled
SIGGRAPH papers by year and by category, and Daniel and many
of the TAs are great resources to bounce ideas off of.

- Bump mapping or displacement mapping (not both) with ability
to enable/disable
- Piecewise Bezier surfaces
- Move the camera or objects in the scene along a path defined
as a piecewise Bezier curve, for example to render a ride on a
roller coaster
- Collision detection (bounding boxes/spheres or polygon level),
possibly including rotational physics
- Shadow mapping, with ability your code to toggle shadows
on/offs
- Shadow volumes, with ability toggle shadows on/off
- Depth of field
- Deferred lighting
- Ambient occlusion (for example, SSAO or screen space ambient
occlusion)
- Anti-aliasing (for example, FXAA or fast approximate antialiasing)
- Fractal terrain
- Fractal plants or trees
- Complex, procedurally modeled city or other environment (more
than just a bunch of cubes or other simple geometric shapes)
- Lindenmayer systems
- GPU raytracing
- HDR, perhaps with bloom
- Non-photorealistic or stylized rendering
- Computer generated art

Here are some more challenging features that you can also attempt
and have been successfully done in the past but we are warning you!
Many of them require a strong background in differential equations
or numerical analysis.

- Path tracing (do not do this unless you are sure you will not
take CS2240!)
- Physically based modeling (cloth, hair, etc.)
- Particle-based fluid simulation (water, smoke, clouds) or 2D
fluid simulation

Here are some examples of technical features that you have already
seen in the assignments that you can easily add to your project.

- Glass, metal shaders
- GPU ray marching
- Texture mapping
- Environment mapping, with ability to enable/disable environment map
- Particle generation
- Procedural geometric shapes like cone, cube, sphere, cylinder,
etc.

<b>Real-time</b>. If you plan to use real-time rendering your project should run at
a minimum of 24 frames per second. This doesn’t apply to you if
your project does not call for the use of OpenGL or CUDA to render
in any way. If it is impossible for your project (e.g. more complex
simulations) to run in real-time but you would still like to demo it as a video, you can use a tool called [ffmpeg](https://stackoverflow.com/questions/24961127/how-to-create-a-video-from-images-with-ffmpeg) to make a video from
images grabbed from the framebuffer

<b>Offline graphics</b>. Another option is to render a cool image / animation sequence using offline graphics.
This can extend your raytracer or, if contributions are on the modeling/animation side, use an
existing offline renderer (e.g. Arnold, Blender Cycles).

<b>Non-Graphics Features</b>. Although we will appreciate and enjoy
any additional features you choose to add to your project, we are
not requiring that you do so. We request that you spend any extra
time that you have implementing more advanced graphical features,
rather than sound effects, music, game logic, etc.

<b>Final Presentations</b>. Presentations will take place on Thursday,
12/16 at 10:30 a.m. in the class. Your team will give a
brief 3-5 minute presentation explaining what you did and showing
your implementation to the class. Again, we ask each team to record
their demo ahead of time with a screen-recording application and
submit it via this [form](https://docs.google.com/forms/d/e/1FAIpQLSdFPUqWM_5_cC89hLj1AFjJ753wmWdCAqdXRZK-woz_YVjR2A/viewform?usp=sf_link) before the presentation day. If you are not
able to make it to the class due to conflicts,
time zone difficulties, or travel plans, you can also pre-record your presentation and
submit it ahead of time.

<b>Implementation</b>. This project must be implemented in C++ and
optionally, OpenGL/GLSL and Qt. Exceptions can be made with
TA approval. We allow and encourage you to re-use the code you
wrote for the earlier projects in this semester, including labs and the
support code used for each lab. You are permitted to look at code
from books and online sources, but you may not copy others’ code
and call it your own. You may use third party libraries if you wish,
however, they must be cleared with your mentor TA first. Keep in
mind that your use of a third party library does not decrease the
amount of work we expect you to do.

<b>Theme</b>. Some of the recent labs provide environment maps that give
the feel of a room or outdoor space. Your project should similarly
have some sort of theme. You can create a simple environment map,
or you can model an environment using 3D geometry. You can also
experiment with combinations of both, for example, by creating a
skybox. The theme may also provide basic interaction controls to
the user. Here are some ideas for themes:

- A planet modeled using fractal terrain, with a sky box.
- A museum room. Include artwork as texture maps. Include
lights, benches, wood floors, rugs, doors, unicorns, etc. if you
wish. The user should be able to navigate the scene and change
view direction with keyboard or mouse controls.
- Build a virtual roller coaster. Let the user’s viewpoint follow along (in or behind) the roller coaster car. Include some
interesting scenery.
- Render a number of marbles bouncing around and bumping into
each other, following the laws of physics, and casting shadows
on each other and the surface they are on. You might want to
apply a texture map to the surface and include a skybox or a
fog in the distance.

You can also check out some past final projects on CS1230’s [YouTube
channel](https://www.youtube.com/channel/UCGNsmYJfDhLMgmAp6VJ_suw/videos) or our other [Youtube channel](https://www.youtube.com/channel/UCyFsTjzWjJ5AkbpcN2ZZ7dA/videos) (We have no clue why there
is two).

## Timeline/Due Dates

- By Monday, 11/15 at 11:59 p.m. EST: Form groups. Submit the
teammate matching [form](https://docs.google.com/forms/d/e/1FAIpQLSdA3AgdyY9TzdxqfcQjUM2lnHrs8RDrgrOu2aX4gRAzv_ZckQ/viewform?usp=sf_link) if you have not found/do not expect
to find a group.
- By Tuesday, 11/23 at 11:59 p.m. EST: Project proposals
submitted via this [form](https://docs.google.com/forms/d/e/1FAIpQLSdZvFYnTE-iSV51DftaD_8joTQNNiA6j-7FCYTfP4CTZvmxIA/viewform?usp=sf_link).
- By Tuesday, 11/30 at 11:59 p.m. EST: Meet with mentor TA
for design check (make sure to finalize and upload your project
plans before this meeting).
- By Thursday, 12/9 at 11:59 p.m. EST: Meet with mentor TA for
project status updates.
- On Thursday, 12/15 at 11:59 p.m. EST: Have the demo video
recorded and submitted via this [form](https://docs.google.com/forms/d/e/1FAIpQLSdFPUqWM_5_cC89hLj1AFjJ753wmWdCAqdXRZK-woz_YVjR2A/viewform?usp=sf_link) for final presentations day
on Thursday, 12/16 at 10:30 a.m. EST.


## Grading

Your grade for this project will be determined by several factors.

<b>Technical excellence.</b> How well did you accomplish what you set
out to do? Is your implementation stable (i.e. bug free and does
not crash)? Does your project represent a significant, yet reasonable
extension on the topics covered in the labs and assignments? We
also expect that you will do something non-trivial. Your mentor TA
will be able to guide you here.

<b>Artistic quality/creativity/theme</b>. Does your demo look and
feel cohesive? You don’t have to create a full-fledged environment,
but we also won’t be impressed with a single shape in the center
of the screen. For example, if you’re going to make an awesome
shader, compliment it with a compelling environment map, like
we do in the labs. We also encourage things like nice 3D models,
nice textures (consider taking digital photographs and converting
them to textures), thoughtful choice of colors and materials, careful
positioning of camera and lights, significant user interaction, etc.

<b>Presentation</b>. Does it look like your presentation was thrown
together at the last minute? Is your presentation sensible and wellorganized? Were you able to effectively explain your implementation
in the 3 minute period?

<b>A note about partners</b>. By default, all members of a group will
get the same grade. If you have any problems with a teammate,
you must tell us about them as soon as the problem occurs; we will
not make retroactive adjustments to grades due to partner issues we
didn’t know about until the last minute. Keep in mind that because
we are using Github this year we will be able to see how much each
partner contributes to the project.


## Where do I start?

There are many ways you can start the final project. You can start
from any of the lab stencil codes or the project repo if you find
them useful for the features you want to implement. For example
Lab 5 or Lab 6 is a good starting point if all you need is a basic
shader program with/without FBOs. Besides that, we also provide
an optional starter code for this project, which you can obtain either
through the Github Classroom assignment or by running:

```
git clone https://github.com/cs123tas/final-stencil.git
```

The starter code creates an OpenGL window with mouse and keyboard input events using Qt. It’s also recommended that you copy
your gl directory from your lab06 into the support code. Once
again, feel free to augment the support code.

Here’s also a general guideline to start.

- Do some research on your own. Ask Daniel or the TAs about
interesting papers and general resources. Take a look at the
CS1230 Youtube channel for inspiration. There are great examples of really good final projects from past years. There are also
amazing projects on Shadertoy that implement cool features.
Also, ask the TAs about VR/AR projects!
- Form groups of two or three. Discuss your ideas for potential
themes / features that you’d like to implement. Try to find
teammates that are interested in the same theme as you!
- Decide on a theme and which features your group will be implementing.
- Do research on the specific features that you would like to
implement, and understand the steps in actually implementing
the feature. Try to explain the steps to someone else, a teammate
for example.
- Talk to your teammates about how all your code will come
together!

This should give you a good idea of where to start, but feel free to
ask the TAs / your mentor TA if you run into any other questions!

## Handing In

To hand in all you’ll need to do is submit your project proposal
and final demo video, which we will use to help grade your final
project if needed. Do create a README file that describes how to
use the program and details any bugs or known issues. Make sure
you thoroughly test and debug your program to make sure it does
not crash or behave unexpectedly.

## The "Oh God Nothing Is Working" FAQ

Many final projects will make use of OpenGL, so here is a list
of common issues students may find when working on their own
OpenGL projects from scratch. Some general advice is to code
incrementally and to make sure you are able to visualize your results
before anything else so that you can test your code.
When programming in OpenGL, sometimes you’ll get a black screen.
There are a number of causes for this, including but not limited to:

- Drawing triangles clockwise instead of counter-clockwise
    - To debug, try reversing the winding order or using `glDisable(GL_CULL)`
- Camera facing the wrong direction, or near/far planes don’t
contain object being rendered
    - To debug, try configuring your camera so you can view a
single triangle in the center of the world
- Data being sent incorrectly to the GPU
    - You might see a black screen if your arguments in `glVertexAttribPointer`, `glBufferData`, or `glDrawArrays` don’t match the size or organization of your VBO. It’s also worth triple
    checking your uniforms. Try simplifying by rendering a
    single triangle.
- Lights misconfigured, or incorrect normal vectors
    - To debug, try setting `glClearColor` to any color but black.
    Alternatively, try having your fragment shader output a
    solid, bright color. It’s possible your objects are appearing,
    but being shaded in all black due to issues with lighting.
- General tip: if you’re having troubles with complex shapes, just
try drawing one triangle and see if you can get that to render,
and build from there. As always, you can post on ED or
contact your mentor TA if you’re really struggling and you’ve
already tried all of these things.

