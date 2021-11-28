#ifndef BASETESSELATOR_H
#define BASETESSELATOR_H

#include <vector>
#include <glm/glm.hpp>

class BaseTesselator
{
public:
    BaseTesselator();
    virtual ~BaseTesselator();

    // Tesselate the actual shape if needed.
    virtual void tesselate() = 0;

    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getVNormals() const;
    std::vector<glm::vec2> getUVCoords() const;

protected:
    bool m_cacheAvailable;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_vNormals;
    std::vector<glm::vec2> m_uvCoords;

    // The number of vertices of this shape.
    // The subclass must override this method
    virtual int verticesNum() = 0;

    // Setup the tesselator's cache.
    // In other words, resize "m_vertices" and "m_vNormals" to their expected size.
    void setup();
};

#endif // BASETESSELATOR_H
