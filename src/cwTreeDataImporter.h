#ifndef CWTREEDATAIMPORTER_H
#define CWTREEDATAIMPORTER_H

//Qt includes
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include "cwTask.h"

//Our includes
#include "cwStation.h"
#include "cwTreeImportData.h"

class cwTreeDataImporter : public cwTask
{
Q_OBJECT

public:
    cwTreeDataImporter(QObject* parent = nullptr);

    virtual bool hasParseErrors();
    virtual QStringList parseErrors();

    virtual bool hasImportErrors();
    virtual QStringList importErrors();

    virtual QString lastImport();

    virtual cwTreeImportData* data();

public slots:
    virtual void setInputFiles(QStringList filenames) = 0;

protected:
    virtual void runTask();
};

#endif // CWTREEDATAIMPORTER_H
