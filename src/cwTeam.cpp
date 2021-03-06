/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

//Our includes
#include "cwTeam.h"

//Qt includes
#include <QByteArray>
#include <QDebug>

cwTeam::cwTeam(QObject *parent) :
    QAbstractListModel(parent)
{
}

/**
  Copy constructor for the team
  */
cwTeam::cwTeam(const cwTeam& team) :
    QAbstractListModel(nullptr)
{
    //Copy the data
    Team = team.Team;
}


void cwTeam::addTeamMember(const cwTeamMember& teamMember) {
    beginInsertRows(QModelIndex(), Team.size(), Team.size());
    Team.append(teamMember);
    endInsertRows();
}

void cwTeam::setTeamMembers(QList<cwTeamMember> team) {
    beginResetModel();
    Team = team;
    endResetModel();
}

int cwTeam::rowCount(const QModelIndex &/*parent*/) const {
    return Team.size();
}

QVariant cwTeam::data(const QModelIndex &index, int role) const {
    const cwTeamMember& teamMember = Team.at(index.row());
    switch(role) {
    case NameRole:
        return teamMember.name();
    case JobsRole:
        return teamMember.jobs();
    }
    return QVariant();
}

void cwTeam::setData(int row, cwTeam::TeamModelRoles role, const QVariant &data) {
    QModelIndex modelIndex = index(row);
    setData(modelIndex, data, role);
}

QHash<int, QByteArray> cwTeam::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[JobsRole] = "jobs";
    return roles;
}

bool cwTeam::setData(const QModelIndex& index, const QVariant &data, int role) {
    if(!index.isValid()) {
        return false;
    }

    switch(role) {
    case NameRole: {
        cwTeamMember& teamMember = Team[index.row()];
        teamMember.setName(data.toString());
        emit dataChanged(index, index);
        return true;
    }
    case JobsRole: {
        if(data.canConvert<QStringList>()) {
            cwTeamMember& teamMember = Team[index.row()];
            QStringList jobList = data.toStringList();
            teamMember.setJobs(jobList);
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    default:
        return false;
    }

}

/**
  This removes the team member from the team
  */
void cwTeam::removeTeamMember(int row) {
    QModelIndex index = this->index(row);
    if(!index.isValid()) { return; }

    beginRemoveRows(QModelIndex(), row, row);
    Team.removeAt(row);
    endRemoveRows();
}
