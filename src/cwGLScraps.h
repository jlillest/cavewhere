/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

#ifndef CWGLSCRAPS_H
#define CWGLSCRAPS_H

//Our includes
#include "cwGLObject.h"
#include "cwTriangulatedData.h"
#include "cwImageTexture.h"
#include "cwGeometryItersecter.h"
class cwCavingRegion;
class cwProject;
class cwScrap;

//Qt includes
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QSharedPointer>

class cwGLScraps : public cwGLObject
{
    Q_OBJECT

    Q_PROPERTY(cwProject* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

public:
    explicit cwGLScraps(QObject *parent = 0);

    cwProject* project() const;
    void setProject(cwProject* project);

    void setCavingRegion(cwCavingRegion* region);

    void initialize();
    void draw();
    void updateData();

    void addScrapToUpdate(cwScrap* scrap);
    void removeScrap(cwScrap* scrap);

    bool visible() const;
    void setVisible(bool visible);

signals:
    void projectChanged();
    void visibleChanged();

private:
    class PendingScrapCommand {
    public:
        enum Type {
            AddScrap,
            RemoveScrap,
            Unknown
        };

        PendingScrapCommand() :
            CommandType(Unknown),
            Scrap(nullptr)
        {

        }

        PendingScrapCommand(Type type, cwScrap* scrap, cwTriangulatedData data) :
            CommandType(type),
            Scrap(scrap),
            Data(data)
        { }

        Type type() const { return CommandType; }
        cwScrap* scrap() const { return Scrap; }
        cwTriangulatedData triangulatedData() const { return Data; }

    private:
        Type CommandType;
        cwScrap* Scrap;
        cwTriangulatedData Data;

    };

    class GLScrap {

    public:
        GLScrap();
        GLScrap(const cwTriangulatedData& data, cwProject* project);

        QOpenGLBuffer PointBuffer;
        QOpenGLBuffer IndexBuffer;
        QOpenGLBuffer TexCoords;

        int NumberOfIndices;
        int ScrapId; //For intersection

        cwImageTexture* Texture;

        void update(const cwTriangulatedData& data);

        void releaseResources();

    };

    cwProject* Project; //!< The project file for loading textures

    //Pending data to update
    QHash<cwScrap*, PendingScrapCommand> PendingChanges;

    //Data in the rendering thread
    QOpenGLShaderProgram* Program;
    int UniformModelViewProjectionMatrix;
    int UniformScaleTexCoords;
    int vVertex;
    int vScrapTexCoords;
    QHash<cwScrap*, GLScrap> Scraps;
    int MaxScrapId;

    bool Visible; //!< True if the scraps are visible and false if they're not

    void initializeShaders();

};

/**
Gets project
*/
inline cwProject* cwGLScraps::project() const {
    return Project;
}

/**
Gets visible
*/
inline bool cwGLScraps::visible() const {
    return Visible;
}

#endif // CWGLSCRAPS_H
