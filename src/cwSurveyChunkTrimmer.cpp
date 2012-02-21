#include "cwSurveyChunkTrimmer.h"

cwSurveyChunkTrimmer::cwSurveyChunkTrimmer(QObject *parent) :
    QObject(parent),
    Chunk(NULL)
{
}

void cwSurveyChunkTrimmer::setChunk(cwSurveyChunk *chunk) {
    if(Chunk == chunk) { return; }

    if(Chunk != NULL) {
        trim(FullTrim);
        disconnect(Chunk, NULL, this, NULL);
    }

    Chunk = chunk;

    if(Chunk != NULL) {
        connect(Chunk, SIGNAL(stationsAdded(int,int)), SLOT(addLastEmptyStation()));
        connect(Chunk, SIGNAL(dataChanged(cwSurveyChunk::DataRole,int)), SLOT(addLastEmptyStation()));

        addLastEmptyStation();
    }

    emit chunkChanged();
}

/**
  This removes the last empty stations and shots from the chunk.

  This will remove all empty station and shots from the end of the survey,
  even from the first two

  chunk - The that's will be trimmed
  */
void cwSurveyChunkTrimmer::trim(cwSurveyChunk *chunk)
{
    //Remove the 2 station if they are invalid
    if(chunk->stationCount() <= 2) {
        if(isStationShotEmpty(chunk, 1)) {
            chunk->removeStation(1, cwSurveyChunk::Above);
        }

        if(isStationShotEmpty(chunk, 0)) {
            chunk->removeStation(0, cwSurveyChunk::Above);
        }
    }

    trim(chunk, FullTrim);
}

/**
  This removes the last empty stations and shots from the chunk

  chunk - The that's will be trimmed
  fullTrim - If true, this will fully trim, else it'll leave just the last
  empty shot and station
  */
void cwSurveyChunkTrimmer::trim(TrimType fullTrim) {
    trim(Chunk, fullTrim);
}

/**
  This removes the last empty stations and shots from the chunk

  chunk - The that's will be trimmed
  fullTrim - If true, this will fully trim, else it'll leave just the last
  empty shot and station
  */
void cwSurveyChunkTrimmer::trim(cwSurveyChunk *chunk, cwSurveyChunkTrimmer::TrimType trimType)
{
    if(chunk->stationCount() <= 2) {
        return;
    }

    switch(trimType) {
    case FullTrim: {
        for(int i = chunk->stationCount() - 1; i > 1; i--) {
            if(isStationShotEmpty(chunk, i)) {
                chunk->removeStation(i, cwSurveyChunk::Above);
            }
        }
        break;
    }
    case PreserveLastEmptyOne: {
        for(int i = chunk->stationCount() - 1; i > 1; i--) {
            //Look a head one
            if(isStationShotEmpty(chunk, i - 1)) {
                //Remove the current
                if(isStationShotEmpty(chunk, i)) {
                    chunk->removeStation(i, cwSurveyChunk::Above);
                }
            }
        }
        break;
    }
    }
}

/**
  This addes an empty station and shot to the end of the surveyChunk
  */

void cwSurveyChunkTrimmer::addLastEmptyStation() {
    trim(PreserveLastEmptyOne);

    //If last station isn't empty, add new one
    if(!isStationShotEmpty(Chunk, Chunk->stationCount() - 1)) {
        connect(Chunk, SIGNAL(stationsAdded(int,int)), this, SLOT(addLastEmptyStation()));
        Chunk->appendNewShot();
        connect(Chunk, SIGNAL(stationsAdded(int,int)), SLOT(addLastEmptyStation()));
    }
}

/**
    Checks to see if the last station and shot is empty

    This function is invalid if the stationIndex is equal to zero, always returns false

    chunk - The chunk that's checked
    stationIndex - The station that's chuck.
  */
bool cwSurveyChunkTrimmer::isStationShotEmpty(cwSurveyChunk *chunk, int stationIndex) {
    if(stationIndex == 0) { return false; }

    cwStation station = chunk->station(stationIndex);
    cwShot shot = chunk->shot(stationIndex - 1);

    return station.name().isEmpty() &&
            station.leftInputState() == cwDistanceStates::Empty &&
            station.rightInputState() == cwDistanceStates::Empty &&
            station.downInputState() == cwDistanceStates::Empty &&
            station.upInputState() == cwDistanceStates::Empty &&
            shot.distanceState() == cwDistanceStates::Empty &&
            shot.compassState() == cwCompassStates::Empty &&
            shot.clinoState() == cwClinoStates::Empty &&
            shot.backCompassState() == cwCompassStates::Empty &&
            shot.backClinoState() == cwClinoStates::Empty;
}
