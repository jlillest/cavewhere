#ifndef CWGLSCRAPS_H
#define CWGLSCRAPS_H

//Our includes
#include "cwGLObject.h"
#include "cwTriangulatedData.h"
#include "cwImageTexture.h"
class cwCavingRegion;
class cwProject;

//Qt includes
#include <QGLShaderProgram>
#include <QGLBuffer>

class cwGLScraps : public cwGLObject
{
    Q_OBJECT


public:
    explicit cwGLScraps(QObject *parent = 0);

    Q_PROPERTY(cwProject* project READ project WRITE setProject NOTIFY projectChanged)

    cwProject* project() const;
    void setProject(cwProject* project);


    void updateGeometry();
    void setCavingRegion(cwCavingRegion* region);

    void initialize();
    void draw();
    
signals:
    void projectChanged();

public slots:

private:
    class GLScrap {

    public:
        GLScrap();
        GLScrap(const cwTriangulatedData& data, cwProject* project);

        QGLBuffer PointBuffer;
        QGLBuffer IndexBuffer;
        QGLBuffer TexCoords;

        int NumberOfIndices;

        QSharedPointer<cwImageTexture> Texture;

    };

    cwProject* Project; //!< The project file for loading textures
    cwCavingRegion* Region;

    QGLShaderProgram* Program;
    int UniformModelViewProjectionMatrix;
    int vVertex;
    int vScrapTexCoords;

    QList<GLScrap> Scraps;

    void initializeShaders();

};

inline void cwGLScraps::setCavingRegion(cwCavingRegion *region) {
    Region = region;
}

/**
Gets project
*/
inline cwProject* cwGLScraps::project() const {
    return Project;
}

#endif // CWGLSCRAPS_H