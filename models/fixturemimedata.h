#ifndef FIXTUREMIMEDATA_H
#define FIXTUREMIMEDATA_H

#include "fixturedetails.h"
#include "recycler/recyclermimedata.h"

#include <QPoint>
#include <QModelIndex>

/*struct FixtureChannel {
    QString name;

    FixtureChannel(QString name) { this->name = name; }
    FixtureChannel() { this->name = "INVALID"; }
};

struct Fixture {
    QString name;
    QVector<FixtureChannel> channels;
};*/

class FixtureMimeData : public RecyclerMimeData
{
    Q_OBJECT
public:
    explicit FixtureMimeData(const ActiveFixture & fixture, int dataPos = 1);

    bool hasFormat(const QString &format) const override;

    ActiveFixture fixture;
    QPoint hotspot;
    int sourceDataPos;

    static QString channelFormat() { return "application/channel-slider-data"; }
};

#endif // FIXTUREMIMEDATA_H
