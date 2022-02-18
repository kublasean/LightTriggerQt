#ifndef OPENFIXTUREFILEREADERTHREAD_H
#define OPENFIXTUREFILEREADERTHREAD_H

#include <QThread>

class OpenFixtureFileReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenFixtureFileReaderThread(const QString &absoluteFilePath, QObject *parent = nullptr);
    ~OpenFixtureFileReaderThread();

    void run() override;

signals:
    void parsedDetails(const QString &dim,
                       const QString &weight,
                       const QString &power,
                       const QString &conn);

    void errorMessage(QString error);

private:
    void parseError(const QString &parseMessage);

    QString path;
};

#endif // OPENFIXTUREFILEREADERTHREAD_H
