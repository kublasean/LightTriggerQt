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

FixtureCapability parseJsonCapabilty(const QJsonObject &json)
{
    FixtureCapability cap;

    if (json.isEmpty())
        return cap;

    QJsonValue val = json.value("type");
    if (!val.isString())
        return cap;

    cap.typeName = val.toString();
    cap.isValid = true;
    return cap;
}

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

    FixtureDetails fixture;

    // Name
    val = root.value("name");
    if (val.isUndefined() || !val.isString()) {
        parseError("No fixture name");
        return;
    }
    fixture.name = val.toString();

    // Parse physical properties if present
    val = root.value("physical");
    if (!val.isUndefined() && val.isObject()) {
        QJsonObject node = val.toObject();
        fixture.dimensions = dimensionsToString(node.value("dimensions"));
        fixture.weight = weightToString(node.value("weight"));
        fixture.power = powerToString(node.value("power"));
        fixture.connector = node.value("DMXconnector").toString();
    }

    // Available channels
    val = root.value("availableChannels");
    if (!val.isObject()) {
        parseError("Malformed available channels");
        return;
    }

    QJsonObject channelsObj = val.toObject();
    for (auto it = channelsObj.begin(); it != channelsObj.end(); it++) {
        QString channelKey = it.key();
        if (!it.value().isObject())
            continue;

        QJsonObject channelObj = it.value().toObject();

        // Single capability
        FixtureCapability cap = parseJsonCapabilty(channelObj.value("capability").toObject());
        if (cap.isValid) {
            FixtureChannel channel;
            channel.capabilities.append(cap);
            channel.name = it.key();
            fixture.availableChannels.insert(it.key(), channel);
            continue;
        }

        // Multiple capabilities
        QJsonArray capArray = channelObj.value("capabilities").toArray();
        FixtureChannel channel;
        channel.name = it.key();
        for (int i=0; i<capArray.size(); i++) {
            cap = parseJsonCapabilty(capArray[i].toObject());
            if (cap.isValid) {
                channel.capabilities.append(cap);
            }
        }
        if (!channel.capabilities.isEmpty()) {
            fixture.availableChannels.insert(it.key(), channel);
        }
    }

    emit parsedDetails(fixture);
}
