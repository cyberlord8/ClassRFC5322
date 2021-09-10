#include "classrfc5322.h"

ClassRFC5322::ClassRFC5322(QObject *parent) : QObject(parent)
{

}

void ClassRFC5322::parseBody(const QString &line, RFC5322MessageStruct &message)
{
    message.body.append(line.toLocal8Bit()+"\r\n");
}

void ClassRFC5322::parseHeader(const QString &line, RFC5322MessageStruct &message)
{
    QStringList headerLine;
    RFC5322HeaderFieldStruct headerField;

    headerLine = line.split(": ");
    headerField.fieldName = headerLine.at(0);
    headerField.fieldBody = " "+headerLine.at(1);
    message.headerFields.append(headerField);
}

QString ClassRFC5322::getJulianDate()
{
    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    QDate currentDate = currentDateTime.date();
    QTime currentTime = currentDateTime.time();
    return QString("%1%2").arg(currentDate.dayOfYear(), 3, 10, QChar('0')).arg(currentTime.toString("hhmmss"));
}

QString ClassRFC5322::removeAllWhiteSpace(QString string)
{
    QString returnString;

    returnString = string;
    while (returnString.contains(" ")) {
        returnString.remove(" ");
    }
    return returnString;
}

QString ClassRFC5322::getDomainAddress(QString string)
{
    //looking for text between < and >
    QString returnString = string;

    returnString = returnString.remove(0,returnString.indexOf('<'));


    return returnString;
}

void ClassRFC5322::parseMessage(const QString &receivedData, RFC5322MessageStruct &message)
{
    PARSE_STATE parseState = NONE;

    QStringList receivedDatList = receivedData.split("\r\n");
    //    qDebug() << receivedDatList;

    foreach (QString line, receivedDatList) {
        if(parseState < BODY && line.contains(':')){//header?
            parseState = HEADER;
        }
        else if (parseState < BODY && line.startsWith(" ")) {
            parseState = FWS;
        }
        else {
            parseState = BODY;
        }
        //TODO ATTACHMENTs??

        switch (parseState) {
        case HEADER:
            //            qDebug() << "HEADER" << line << line.indexOf(QLatin1Char(':'));
            parseHeader(line, message);
            break;
        case FWS://folded white space
            message.headerFields.last().fieldBody.append("\r\n"+line);
            break;
        case BODY:
            //            qDebug() << "BODY" << line;
            parseBody(line, message);
            break;
        default:
            break;
        }
    }

}

QByteArray ClassRFC5322::composeMessage(const RFC5322MessageStruct message)
{
    QByteArray returnArray;

    for (int i=0; i<message.headerFields.size() ; i++) {
        qDebug() << message.headerFields.at(i).fieldName << ":" << message.headerFields.at(i).fieldBody;
        returnArray.append(message.headerFields.at(i).fieldName.toLocal8Bit() + ": " +
                           message.headerFields.at(i).fieldBody.toLocal8Bit() + "\r\n");

    }

    qDebug() << message.body;
    returnArray.append("\r\n" + message.body + "\r\n");
    qDebug() << "";

    return returnArray;
}

QString ClassRFC5322::generateMessageID(QString from, int msgNumber)
{
    QString returnString;
    QString julianDate = getJulianDate();
    returnString.append(julianDate+"."+QString::number(msgNumber)+"."+from.simplified());
    return returnString;
}

QString ClassRFC5322::generateMessageID(QString from)
{
    QString returnString;
    returnString.append(getJulianDate()+"."+getDomainAddress(from)+"\r\n ");
    return returnString;
}

QByteArray ClassRFC5322::generateDigest(QString messageBody)
{
    return QCryptographicHash::hash(messageBody.toLocal8Bit().simplified(), QCryptographicHash::Md5).toHex();
}//composeMessage
