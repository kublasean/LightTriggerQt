#ifndef FIXTUREDETAILS_H
#define FIXTUREDETAILS_H

#include <QObject>
#include <QString>
#include <QMap>

struct FixtureCapability {
    QString typeName;

    bool isValid() const { return !typeName.isNull() && !typeName.isEmpty(); }
};

struct FixtureChannel {
    QString name;
    QList<FixtureCapability> capabilities;

    bool isValid() const { return !name.isNull() && !name.isEmpty() && !capabilities.isEmpty(); }
};

struct FixtureMode {
    QString name;
    QList<FixtureChannel> channels;

    bool isValid() const { return !name.isNull() && !name.isEmpty() && !channels.isEmpty(); }
};

struct FixtureDetails {
    QString name;
    QString dimensions;
    QString weight;
    QString power;
    QString connector;

    QList<FixtureMode> modes;

    bool isValid() const { return !name.isNull() && !name.isEmpty() && !modes.isEmpty(); }
};

Q_DECLARE_METATYPE(FixtureDetails);


#endif // FIXTUREDETAILS_H
