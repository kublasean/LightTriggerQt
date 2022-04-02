#include "fixturedetailscache.h"
#include "openfixturefilereaderthread.h"

FixtureDetailsCache::FixtureDetailsCache(QObject *parent)
    : QObject{parent}
{

}

void FixtureDetailsCache::getFixture(QString absoluteFixtureFilePath, FixtureDetailsReceiver *receiver)
{
    if (cache.contains(absoluteFixtureFilePath)) {
        emit receiver->parsedDetails(cache.value(absoluteFixtureFilePath));
        return;
    }

    OpenFixtureFileReaderThread *parserThread = new OpenFixtureFileReaderThread(absoluteFixtureFilePath, this);
    connect(parserThread, &QThread::finished, parserThread, &QObject::deleteLater);
    connect(parserThread, &OpenFixtureFileReaderThread::parsedDetails, receiver, &FixtureDetailsReceiver::parsedDetails);
    connect(parserThread, &OpenFixtureFileReaderThread::parsedDetails, this, &FixtureDetailsCache::addToCache);
    connect(parserThread, &OpenFixtureFileReaderThread::errorMessage, receiver, &FixtureDetailsReceiver::errorMessage);
    parserThread->start();
}

void FixtureDetailsCache::addToCache(const FixtureDetails & details)
{
    cache.insert(details.absolutePath, details);
}


