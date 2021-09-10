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
//    static PARSE_STATE parseState;

    static void parseBody(const QString &line, RFC5322MessageStruct &message);
    static void parseHeader(const QString &headerLine, RFC5322MessageStruct &message);
    static QString getJulianDate();
    static QString removeAllWhiteSpace(QString string);
    static QString getDomainAddress(QString string);

public:
    explicit ClassRFC5322(QObject *parent = nullptr);

    static void parseMessage(const QString &receivedData, RFC5322MessageStruct &message);
    static QByteArray composeMessage(const RFC5322MessageStruct message);

    static QString generateMessageID(QString from, int msgNumber);
    static QString generateMessageID(QString from);
    static QByteArray generateDigest(QString messageBody);
signals:

};

#endif // CLASSRFC5322_H
