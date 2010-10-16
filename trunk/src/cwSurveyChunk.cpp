//Our includes
#include "cwSurveyChunk.h"
#include "cwStation.h"
#include "cwShot.h"


//Qt includes
#include <QHash>
#include <QDebug>
#include <QVariant>

cwSurveyChunk::cwSurveyChunk(QObject * parent) :
        QObject(parent)
{

}

/**
  \brief Checks if the survey Chunk is valid
  */
bool cwSurveyChunk::IsValid() {
    return (Stations.size() - 1) == Shots.size() && !Stations.empty();
}

int cwSurveyChunk::StationCount() {
    return Stations.count();
}

cwStation* cwSurveyChunk::Station(int index) {
    if(StationIndexCheck(index)) {
        return Stations[index];
    }
    return NULL;
}

int cwSurveyChunk::ShotCount() {
    return Shots.size();
}

cwShot* cwSurveyChunk::Shot(int index) {
    if(ShotIndexCheck(index)) {
        return Shots[index];
    }
    return NULL;
}

/**
  \brief Adds a station to the survey chunk.

  This will add the station to the end of the list.
  This will also add a shot to the suvey chunk.
  */
void cwSurveyChunk::AppendNewShot() {
    cwStation* fromStation;
    if(Stations.isEmpty()) {
        fromStation = new cwStation();
    } else {
        fromStation = Stations.last();
        if(!fromStation->IsValid()) {
            return;
        }
    }

    cwStation* toStation = new cwStation();
    cwShot* shot = new cwShot();

    AppendShot(fromStation, toStation, shot);
}

/**
  \brief Adds a shot to the chunk

  \param fromStation - The shot's fromStation
  \param toStation - The shot's toStation
  \param shot - The shot

  Use CanAddShot to make sure you can add the shot.
  This function does nothing if the from, to station, or shot are null or the from station
  isn't equal to the last station in the chunk.  If toStation isn't the last station,
  you need to create a new cwSurveyChunk and call this function again
  */
void cwSurveyChunk::AppendShot(cwStation* fromStation, cwStation* toStation, cwShot* shot) {
    qDebug() << "Trying to add shot";
    if(!CanAddShot(fromStation, toStation, shot)) { return; }

    int index;
    int firstIndex = Stations.size();
    if(Stations.empty()) {
        Stations.push_back(fromStation);
    }

    index = Shots.size();
    Shots.push_back(shot);
    emit ShotsAdded(index, index);

    index = Stations.size();
    Stations.push_back(toStation);
    emit StationsAdded(firstIndex, index);
}

/**
  \brief Checks if a shot can be added to the chunk

  \returns true if AddShot() will be successfull or will do nothing
  */
bool cwSurveyChunk::CanAddShot(cwStation* fromStation, cwStation* toStation, cwShot* shot) {
    return fromStation != NULL && toStation != NULL && shot != NULL &&
            (Stations.empty() || Stations.last() == fromStation);
}

/**
  \brief Removes a shot and a station from the chunk

  This will do nothing if the stationIndex is out of bounds.
  \param stationIndex - The station that'll be removed from the model
  \param shot - The shot that'll be removed.  The shot above the station or
  below the station will be remove.  If the shot direction is invalid, ie. can't
  remove the shot, this function will do nothing.
  */
void cwSurveyChunk::RemoveStation(int stationIndex, Direction shot) {
    if(!CanRemoveStation(stationIndex, shot)) { return; }

    //The index to the shot that'll be removed
    int shotIndex = Index(stationIndex, shot);

    //Remove them
    Remove(stationIndex, shotIndex);
}

/**
  \brief Checks to see if the model can remove the station with the shot direction
  \return true if it can and false if it can't
  */
bool cwSurveyChunk::CanRemoveStation(int stationIndex, Direction shot) {
    if(StationCount() <= 2) { return false; }
    if(stationIndex < 0 || stationIndex >= StationCount()) { return false; }
    int shotIndex = Index(stationIndex, shot);
    if(shotIndex < 0 || shotIndex >= ShotCount()) { return false; }

    return true;
}

/**
  \brief Removes a shot and a station from the chunk

  This will do nothing if the shotIndex is out of bounds.
  \param shotIndex - The station that'll be removed from the model
  \param station - The station that'll be removed.  The station above the shot or
  below the shot will be remove.  If the shot direction is invalid, ie. can't
  remove the station, this function will do nothing.
  */
void cwSurveyChunk::RemoveShot(int shotIndex, Direction station) {
    if(!CanRemoveShot(shotIndex, station)) {
        return;
    }

    //The index of the station that'll be removed
    int stationIndex = Index(shotIndex, station);

    //Remove them
    Remove(stationIndex, shotIndex);
}

/**
  \brief Checks to see if the model can remove the shot with the station direction
  \return true if it can and false if it can't
  */
bool cwSurveyChunk::CanRemoveShot(int shotIndex, Direction station) {
    if(ShotCount() <= 1) { return false; }
    if(shotIndex < 0 || shotIndex >= ShotCount()) { return false; }
    int stationIndex = Index(shotIndex, station);
    if(stationIndex < 0 || stationIndex >= StationCount()) { return false; }

    return true;
}

/**
  \brief Removes a station and a shot from the chunk

  This does no bounds checking!!!
  */
void cwSurveyChunk::Remove(int stationIndex, int shotIndex) {
    Stations.removeAt(stationIndex);
    emit StationsRemoved(stationIndex, stationIndex);

    Shots.removeAt(shotIndex);
    emit ShotsRemoved(shotIndex, shotIndex);
}

/**
  \brief Helper to the remove functions.
  */
int cwSurveyChunk::Index(int index, Direction direction) {
    switch(direction) {
    case Above:
        return index - 1;
    case Below:
        return index;
    }
    return -1;
}

