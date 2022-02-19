#ifndef FIXTUREDETAILS_H
#define FIXTUREDETAILS_H

#include <QObject>
#include <QString>
#include <QMap>

struct FixtureCapability {
    QString typeName;
    bool isValid = false;
};

struct FixtureChannel {
    QString name;
    QList<FixtureCapability> capabilities;
};

struct FixtureMode {
    QString name;
    QList<int> channels;
};

struct FixtureDetails {
    QString name;
    QString dimensions;
    QString weight;
    QString power;
    QString connector;

    QMap<QString, FixtureChannel> availableChannels;
    QList<FixtureMode> modes;
};

Q_DECLARE_METATYPE(FixtureDetails);


#endif // FIXTUREDETAILS_H
