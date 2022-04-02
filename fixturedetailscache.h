#ifndef FIXTUREDETAILSCACHE_H
#define FIXTUREDETAILSCACHE_H

#include <QObject>
#include <QMap>

#include "models/fixturedetails.h"

class FixtureDetailsReceiver : public QObject {
    Q_OBJECT
signals:
    void parsedDetails(const FixtureDetails &);
    void errorMessage(const QString &);
};

class FixtureDetailsCache : public QObject
{
    Q_OBJECT
public:
    explicit FixtureDetailsCache(QObject *parent = nullptr);    
    void getFixture(QString absoluteFixtureFilePath, FixtureDetailsReceiver *receiver);

private slots:
    void addToCache(const FixtureDetails &);

private:
    QMap<QString, FixtureDetails> cache;
};

#endif // FIXTUREDETAILSCACHE_H
