/*************************************************************
**
** Copyright (C) 2021 Timothy Millea <cyberlord@cyber-wizard.com>
**
** License: GPLv3.0 or later
**
*************************************************************/

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
    PS_NONE,
    PS_HEADER,
    PS_FWS,
    PS_BODY,
    PS_ATTACHMENT
};

class ClassRFC5322 : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief parseBody - parse the line and store in message.body
     * @param line
     * @param message
     */
    static void parseBody(const QString &line, RFC5322MessageStruct &message);
    /**
     * @brief parseHeader - parse the line and store in message header
     * @param headerLine
     * @param message
     */
    static void parseHeader(const QString &headerLine, RFC5322MessageStruct &message);
    /**
     * @brief getJulianDate
     * @return - the current datatimeUTC in day of year format
     */
    static QString getJulianDate();
    /**
     * @brief removeAllWhiteSpace - remove all white space from string
     * @param string
     * @return
     */
    static QString removeAllWhiteSpace(QString string);
    /**
     * @brief getDomainAddress - get the domain address from string
     * @param string
     * @return
     */
    static QString getDomainAddress(QString string);

public:
    /**
     * @brief parseMessage - parse received data and store in message
     * @param receivedData
     * @param message
     */
    static void parseMessage(const QString &receivedData, RFC5322MessageStruct &message);
    /**
     * @brief parseMessage - parse received data and store in message
     * @param receivedData
     * @return
     */
    static RFC5322MessageStruct parseMessage(const QString &receivedData);
    /**
     * @brief composeMessage - compose RFC5322 message from message structure
     * @param message
     * @return
     */
    static QByteArray composeMessage(const RFC5322MessageStruct message);
    /**
     * @brief generateMessageID - generate a messageID
     * @param from
     * @param msgNumber
     * @return
     */
    static QString generateMessageID(QString from, int msgNumber);
    /**
     * @brief generateMessageID - generate a messageID
     * @param from
     * @return
     */
    static QString generateMessageID(QString from);
    /**
     * @brief generateDigest - generate an MD5 digest from message body
     * @param messageBody
     * @return
     */
    static QByteArray generateDigest(QString messageBody);
    /**
     * @brief getCurrentDate - get the current date in RFC5322 format
     * @return
     */
    static QString getCurrentDate();
    /**
     * @brief getFieldData - get data from fieldname in message struct
     * @param fieldName
     * @param messageStructure
     * @return
     */
    static QString getFieldData(const QString fieldName, RFC5322MessageStruct &messageStructure);
    /**
     * @brief getHeaderData - get header data in RFC5322 format from message struct
     * @param messageStructure
     * @return
     */
    static QString getHeaderData(RFC5322MessageStruct &messageStructure);

signals:

};

#endif // CLASSRFC5322_H
