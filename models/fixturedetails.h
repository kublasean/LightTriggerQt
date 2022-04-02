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
    QString absolutePath;

    QList<FixtureMode> modes;

    bool isValid() const { return !name.isNull() && !name.isEmpty() && !modes.isEmpty(); }
};

struct ActiveFixture {
    FixtureMode mode;
    QString name;
    QString detailsAbsolutePath;
    int id;

    ActiveFixture() {
        name = "";
        detailsAbsolutePath = "";
        id = 1;
    }

    QString fixtureId() const {
        return name + " " + mode.name + " " + QString::number(id);
    }
};

Q_DECLARE_METATYPE(FixtureDetails);


#endif // FIXTUREDETAILS_H
