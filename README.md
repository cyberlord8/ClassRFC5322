# ClassRFC5322
Qt RFC5322 Class - no dependencies

Class to parse and compose RFC5322 Internet Message Format messages.

Static classes may be called ClassRFC5322::methodName(paramaters);

```
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

public:
    static void parseMessage(const QString &receivedData, RFC5322MessageStruct &message);
    static RFC5322MessageStruct parseMessage(const QString &receivedData);
    static QByteArray composeMessage(const RFC5322MessageStruct message);
    static QString generateMessageID(QString from, int msgNumber);
    static QString generateMessageID(QString from);
    static QByteArray generateDigest(QString messageBody);
```
Examples:
```
#include "classrfc5322.h"

void composeRFC5322Message()
{
    RFC5322MessageStruct messageStruct;
    RFC5322HeaderFieldStruct headerField;
    
    messageStruct.body.append("Message text\r\ngoes here.");//add message body text to structure.body
    
    //add all required and optional fields to structure.headerFields
    headerField.fieldName = "MD5";
    headerField.fieldBody = ClassRFC5322::generateDigest(messageStruct.body);
    messageStruct.headerFields.append(headerField);
    
    headerField.fieldName = "Message-ID";
    headerField.fieldBody = ClassRFC5322::generateMessageID("From_Address");
    messageStruct.headerFields.append(headerField);
        
    headerField.fieldName = "From";
    headerField.fieldBody = "From_Address";
    messageStruct.headerFields.append(headerField);
    
    //optional
    headerField.fieldName = "Sender";
    headerField.fieldBody = "Sender_Address";
    messageStruct.headerFields.append(headerField);
    
    headerField.fieldName = "To";
    headerField.fieldBody = "ToAddress";
    messageStruct.headerFields.append(headerField);
    
    //optional
    headerField.fieldName = "Cc";
    headerField.fieldBody = "Cc_Address";
    messageStruct.headerFields.append(headerField);
    
    //add other optional fields if needed
    
    headerField.fieldName = "Date";
    headerField.fieldBody = QDateTime::currentDateTimeUtc().toString(Qt::RFC2822Date);
    messageStruct.headerFields.append(headerField);
    
    headerField.fieldName = "Subject";
    headerField.fieldBody = "Subject text goes here";
    messageStruct.headerFields.append(headerField);
    
    QByteArray rfc5322FormatMessage = ClassRFC5322::composeMessage(messageStruct);
    
    //do something with the QByteArray
    
}//composeRFC5322Message

void parseRFC5322Message(QString receivedMessage){
    RFC5322MessageStruct messageStruct = ClassRFC5322::parseMessage(receivedMessage);

    QString messageBody = messageStruct.body;

    QString fromAddress = ClassRFC5322::getFieldData("From", messageStruct);
    QString subjectText = ClassRFC5322::getFieldData("Subject", messageStruct);
    
    // ...etc...
    
    //do stuff with the data
}//parseRFC5322Message
```

