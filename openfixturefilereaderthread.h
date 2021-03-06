#ifndef OPENFIXTUREFILEREADERTHREAD_H
#define OPENFIXTUREFILEREADERTHREAD_H

#include "models/fixturedetails.h"

#include <QThread>

class OpenFixtureFileReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenFixtureFileReaderThread(const QString &absoluteFilePath, QObject *parent = nullptr);
    ~OpenFixtureFileReaderThread();

    void run() override;

signals:
    void parsedDetails(const FixtureDetails &details);

    void errorMessage(QString error);

private:
    void parseError(const QString &parseMessage);

    QString path;
};

#endif // OPENFIXTUREFILEREADERTHREAD_H
