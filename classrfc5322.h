#ifndef CLASSRFC5322_H
#define CLASSRFC5322_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>

struct RFC5322HeaderFieldStruct
{
    QString fieldName;
    QString fieldBody;

};

struct RFC5322MessageStruct
{
    QByteArray body;
    QList<RFC5322HeaderFieldStruct> headerFields;
    //TODO MIME attachments???
};



enum PARSE_STATE {
    NONE,
    HEADER,
    FWS,
    BODY,
    ATTACHMENT
};

class ClassRFC5322 : public QObject
{
    Q_OBJECT
private:
    static void parseBody(const QString &line, RFC5322MessageStruct &message);
    static void parseHeader(const QString &headerLine, RFC5322MessageStruct &message);
    static QString getJulianDate();
    static QString removeAllWhiteSpace(QString string);
    static QString getDomainAddress(QString string);

public:
    static void parseMessage(const QString &receivedData, RFC5322MessageStruct &message);
    static RFC5322MessageStruct parseMessage(const QString &receivedData);
    static QByteArray composeMessage(const RFC5322MessageStruct message);
    static QString generateMessageID(QString from, int msgNumber);
    static QString generateMessageID(QString from);
    static QByteArray generateDigest(QString messageBody);
    static QString getFieldData(QString fieldName, RFC5322MessageStruct &messageStructure);

signals:

};

#endif // CLASSRFC5322_H
