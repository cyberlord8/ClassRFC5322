/*****************************************************************
**
** Copyright (C) 2021 Timothy Millea <cyberlord@cyber-wizard.com>
**
** License: GPLv3.0 or later
**
*****************************************************************/

#include "classrfc5322.h"

void ClassRFC5322::parseBody(const QString &line, RFC5322MessageStruct &message)
{
    message.body.append(line.toLocal8Bit()+"\r\n");
}//parseBody

void ClassRFC5322::parseHeader(const QString &line, RFC5322MessageStruct &message)
{
    QStringList headerLine;
    RFC5322HeaderFieldStruct headerField;
    headerLine = line.split(": ");
    headerField.fieldName = headerLine.at(0);
    headerField.fieldBody = " "+headerLine.at(1);
    message.headerFields.append(headerField);
}//parseHeader

QString ClassRFC5322::getJulianDate()
{
    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    QDate currentDate = currentDateTime.date();
    QTime currentTime = currentDateTime.time();
    return QString("%1%2").arg(currentDate.dayOfYear(), 3, 10, QChar('0')).arg(currentTime.toString("hhmmss"));
}//getJulianDate

QString ClassRFC5322::removeAllWhiteSpace(QString string)
{
    QString returnString;

    returnString = string;
    while (returnString.contains(" ")) {
        returnString.remove(" ");
    }
    return returnString;
}//removeAllWhiteSpace

QString ClassRFC5322::getDomainAddress(QString string)
{
    //looking for text between < and >
    QString returnString = string;
    returnString = returnString.remove(0,returnString.indexOf('<'));
    return returnString;
}//getDomainAddress

void ClassRFC5322::parseMessage(const QString &receivedData, RFC5322MessageStruct &message)
{
    PARSE_STATE parseState = PS_NONE;
    QStringList receivedDatList = receivedData.split("\r\n");
    foreach (QString line, receivedDatList) {
        if(parseState < PS_BODY && line.contains(':')){//header?
            parseState = PS_HEADER;
        }
        else if (parseState < PS_BODY && line.startsWith(" ")) {
            parseState = PS_FWS;
        }
        else {
            parseState = PS_BODY;
        }
        //TODO ATTACHMENTs??
        switch (parseState) {
        case PS_HEADER:
            parseHeader(line, message);
            break;
        case PS_FWS://folded white space
            message.headerFields.last().fieldBody.append("\r\n"+line);
            break;
        case PS_BODY:
            parseBody(line, message);
            break;
        default:
            break;
        }
    }
}//parseMessage

RFC5322MessageStruct ClassRFC5322::parseMessage(const QString &receivedData)
{
    RFC5322MessageStruct messageStructure;
    PARSE_STATE parseState = PS_NONE;
    QStringList receivedDatList = receivedData.split("\r\n");
    foreach (QString line, receivedDatList) {
        if(parseState < PS_BODY && line.contains(':')){//header?
            parseState = PS_HEADER;
        }
        else if (parseState < PS_BODY && line.startsWith(" ")) {
            parseState = PS_FWS;
        }
        else {
            parseState = PS_BODY;
        }
        //TODO ATTACHMENTs??
        switch (parseState) {
        case PS_HEADER:
            parseHeader(line, messageStructure);
            break;
        case PS_FWS://folded white space
            messageStructure.headerFields.last().fieldBody.append("\r\n"+line);
            break;
        case PS_BODY:
            parseBody(line, messageStructure);
            break;
        default:
            break;
        }
    }
    return messageStructure;
}//parseMessage

QByteArray ClassRFC5322::composeMessage(const RFC5322MessageStruct message)
{
    QByteArray returnArray;
    for (int i=0; i<message.headerFields.size() ; i++) {
        returnArray.append(message.headerFields.at(i).fieldName.toLocal8Bit() + ": " +
                           message.headerFields.at(i).fieldBody.toLocal8Bit() + "\r\n");

    }

    returnArray.append("\r\n" + message.body);
    return returnArray;
}//composeMessage

QString ClassRFC5322::generateMessageID(QString from, int msgNumber)
{
    QString returnString;
    returnString.append(getJulianDate()+"."+QString::number(msgNumber)+"."+getDomainAddress(from));
    return returnString;
}//generateMessageID

QString ClassRFC5322::generateMessageID(QString from)
{
    QString returnString;
    returnString.append(getJulianDate()+"."+getDomainAddress(from));
    return returnString;
}//generateMessageID

QByteArray ClassRFC5322::generateDigest(QString messageBody)
{
    return QCryptographicHash::hash(messageBody.toLocal8Bit().simplified(), QCryptographicHash::Md5).toHex();
}//generateDigest

QString ClassRFC5322::getFieldData(const QString fieldName, RFC5322MessageStruct &messageStructure)
{
    QString returnString;
    for (int i=0; i < messageStructure.headerFields.size() ; i++) {
        if(messageStructure.headerFields.at(i).fieldName == fieldName){
            returnString = messageStructure.headerFields.at(i).fieldBody;
            break;
        }
    }
    return returnString;
}//getFieldData

QString ClassRFC5322::getHeaderData(RFC5322MessageStruct &messageStructure)
{
    QString returnString;
    for (int i=0; i < messageStructure.headerFields.size() ; i++) {
        if(messageStructure.headerFields.at(i).fieldName.startsWith('_'))
            continue;
        returnString.append(messageStructure.headerFields.at(i).fieldName+": ");
        returnString.append(messageStructure.headerFields.at(i).fieldBody+"\r\n");
    }
    return returnString;
}//getHeaderData

QString ClassRFC5322::getCurrentDate()
{
    return QDateTime::currentDateTimeUtc().toString(Qt::RFC2822Date);
}//
