#include "fixturemimedata.h"

FixtureMimeData::FixtureMimeData(const ActiveFixture &fix, int dataPos)
{
    fixture = fix;
    sourceDataPos = dataPos;
    rows = fix.mode.channels.count();
}

bool FixtureMimeData::hasFormat(const QString &format) const
{
    if (FixtureMimeData::channelFormat() == format)
        return true;

    return RecyclerMimeData::hasFormat(format);
}

