#include "openfixturefilereaderthread.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QVersionNumber>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#define SUPPORTED_OPENFIXTURE_MAJOR_VERSION 12

QString dimensionsToString(QJsonValue value)
{
    QJsonArray array = value.toArray();

    QStringList stringList;
    for (int i=0; i<array.size(); i++) {
        stringList << QString::number(array[i].toDouble());
    }

    if (stringList.isEmpty())
        return QString();

    return stringList.join(" x ") + "mm";
}

QString weightToString(QJsonValue value)
{
    return value.isDouble() ? QString::number(value.toDouble()) + "kg" : QString();
}

QString powerToString(QJsonValue value)
{
    return value.isDouble() ? QString::number(value.toDouble()) + "W" : QString();
}

OpenFixtureFileReaderThread::OpenFixtureFileReaderThread(const QString &absoluteFilePath, QObject *parent)
    : QThread{parent}
{
    path = absoluteFilePath;
}

OpenFixtureFileReaderThread::~OpenFixtureFileReaderThread()
{
    qDebug() << "Fixture parser thread destructor";
}

void OpenFixtureFileReaderThread::parseError(const QString &parseMessage)
{
    emit errorMessage("Unable to parse " + path + ": " + parseMessage);
}

void OpenFixtureFileReaderThread::run()
{
    QFile file(path);

    qDebug() << "Parsing" << path;

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        emit errorMessage(file.errorString());
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &error);
    if (jsonDoc.isNull()) {
        qDebug() << error.errorString();
        emit errorMessage(error.errorString());
        return;
    }
    if (!jsonDoc.isObject()) {
        parseError("Not a JSON object");
        return;
    }

    QJsonObject root = jsonDoc.object();
    QJsonValue val = root.value("$schema");
    if (val.isUndefined() || !val.isString()) {
        parseError("No $schema string");
        return;
    }

    QRegularExpression re("schema-(\\d+\\.\\d+\\.\\d+)");
    QRegularExpressionMatch match = re.match(val.toString());
    if (!match.hasMatch()) {
        parseError("Unable to find schema version in schema URL");
        return;
    }

    QVersionNumber schemaVersion = QVersionNumber::fromString(match.captured(1));
    if (schemaVersion.majorVersion() == 0 || schemaVersion.majorVersion() != SUPPORTED_OPENFIXTURE_MAJOR_VERSION) {
        parseError("Unsupported schema version: " + match.captured(1) + ", supported is: " + QString::number(SUPPORTED_OPENFIXTURE_MAJOR_VERSION) + ".0.0");
        return;
    }

    // Parse physical properties if present
    val = root.value("physical");
    if (!val.isUndefined() && val.isObject()) {
        QJsonObject node = val.toObject();
        emit parsedDetails(dimensionsToString(node.value("dimensions")),
                           weightToString(node.value("weight")),
                           powerToString(node.value("power")),
                           node.value("DMXconnector").toString());
    }
}
