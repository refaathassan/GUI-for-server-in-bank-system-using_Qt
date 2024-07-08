#include "serverhandler.h"



ServerHandler::ServerHandler(qint32 ID, QObject *parent): QThread{parent},ID{ID}
{
    PRU1=new LogHandler();
    PRU2=new GetAccounHandler();
    PRU3=new ViewAccountBalanceHandler();
    PRU4=new ViewDataBase();
    PRU5=new DeleteHandler();
    PRU6=new AddUserHandler();
    PRU7=new MakeTransactionHandler();
    PRU8=new TransferAmountHandler();
    PRU9=new ViewTransactionHistoryHandler();
    PRU10=new UpdateUserHandler();
    PRU1->SetNextHandler(PRU2);
    PRU2->SetNextHandler(PRU3);
    PRU3->SetNextHandler(PRU4);
    PRU4->SetNextHandler(PRU5);
    PRU5->SetNextHandler(PRU6);
    PRU6->SetNextHandler(PRU7);
    PRU7->SetNextHandler(PRU8);
    PRU8->SetNextHandler(PRU9);
    PRU9->SetNextHandler(PRU10);

    key = "abcdefghijklmnop"; // 16 bytes for AES 128
    iv = "1234567890123456";  // 16 bytes IV
    publick =QCoreApplication::applicationDirPath()+"//public.pem";
    //connect(PRU1,&Handler::SendToSocket,this,&ServerHandler::SendMassage);
}

void ServerHandler::OnDisconnect()
{
    if(soc->isOpen())
    {
        soc->close();
        //qDebug()<<"client =>"<<ID<<"  Disconnected  "<<Qt::endl;
    }
}

void ServerHandler::OnReadyRead()
{

    QByteArray B_arrary=soc->readAll();
    QByteArray dec_B_arrary=decryptAndVerify(B_arrary,key,iv,publick.toUtf8());
    qDebug()<<"on ready read"<<dec_B_arrary<<"    "<<B_arrary<<Qt::endl;
    //QString str=QString(B_arrary);
    QJsonDocument json=QJsonDocument::fromJson(dec_B_arrary);
    QJsonObject jsonObj=json.object();
    Operation(jsonObj);
    //PRU1->Handling(jsonObj);
    // qint32 length=str.split("/")[0].split(':')[1].toInt();
    // QString data=str.right(length);
    // QJsonDocument json=QJsonDocument::fromJson(data.toUtf8());
    // QJsonObject jsonObj=json.object();
    // qint32 dataSize=jsonObj["size"].toInt();
    // QString realdata=jsonObj["FileData"].toString();
    // if(dataSize==realdata.size())
    // {
    //     Operation(realdata);
    // }

    //Operation(QString(B_arrary));
}

void ServerHandler::SendMassage(QJsonObject json)
{
    if(soc->isOpen())
    {
        //QJsonObject news;
        // news["file-type"]="massage";
        // news["size"]=massage.toUtf8().size();
        // news["FileData"]=massage;
        QByteArray byte=QJsonDocument(json).toJson(QJsonDocument::Compact);
        //QByteArray enc_byte=encryptAES(byte);
        // QString str=QString("JsonSize:%1/").arg(byte.size());
        // byte.prepend(str.toUtf8());
        //qDebug()<<"on ready send"<<byte<<Qt::endl;
        soc->write(byte);
    }
}

void ServerHandler::Operation(QJsonObject json)
{
    // if(massage=="hello")
    // {
    //     SendMassage("massage reply..............");
    // }
   QJsonObject js =PRU1->Handling(json);

   SendMassage(js);

}

void ServerHandler::run()
{
    //qDebug()<<"client=>  "<<ID<<"   is running in thread=> "<<QThread::currentThreadId()<<Qt::endl;
    soc=new QTcpSocket;
    soc->setSocketDescriptor(ID);
    connect(soc,&QTcpSocket::readyRead,this,&ServerHandler::OnReadyRead,Qt::DirectConnection);
    connect(soc,&QTcpSocket::disconnected,this,&ServerHandler::OnDisconnect,Qt::DirectConnection);
    //SendMassage("Hello From My Server...........");
    exec();
}


QByteArray ServerHandler::decryptAndVerify(const QByteArray &encryptedData, const QByteArray &key, const QByteArray &iv, const QByteArray &rsaPublicKeyFile) {
    // Decrypt the base64 encoded data
    QByteArray encrypted = QByteArray::fromBase64(encryptedData);

    // Decrypt the data
    QByteArray decrypted = QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::CBC, encrypted, key, iv, QAESEncryption::PKCS7);
    int padLength = decrypted.at(decrypted.length() - 1);
    decrypted = decrypted.left(decrypted.length() - padLength);
    // Assuming SIGNATURE_LENGTH is known or defined elsewhere in your application
    int SIGNATURE_LENGTH = 256; // Example: adjust according to your signature size (RSA 2048-bit key produces a 256-byte signature)

    // Ensure decrypted data is at least as long as the signature
    if (decrypted.length() <= SIGNATURE_LENGTH) {
        qDebug() << "Invalid decrypted data length";
        return QByteArray();
    }

    // Extract the original data and received signature
    QByteArray originalData = decrypted.left(decrypted.length() - SIGNATURE_LENGTH);
    QByteArray receivedSignature = decrypted.right(SIGNATURE_LENGTH);

    // Verify the signature
    if (!verifySignature(originalData, receivedSignature, rsaPublicKeyFile)) {
        qDebug() << "Signature verification failed!";
        return QByteArray(); // Or handle the verification failure accordingly
    }

    return originalData;
}



bool ServerHandler::verifySignature(const QByteArray &data, const QByteArray &signature, const QByteArray &rsaPublicKeyFile) {
    QFile file(rsaPublicKeyFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open public key file";
        return false;
    }

    QByteArray publicKeyData = file.readAll();
    file.close();

    const char *publicKey = publicKeyData.constData();

    // Read RSA public key
    BIO *bio = BIO_new_mem_buf((void*)publicKey, -1);
    if (!bio) {
        qDebug() << "Failed to create BIO for public key";
        return false;
    }

    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (!rsa) {
        qDebug() << "Failed to load RSA public key";
        BIO_free(bio);
        return false;
    }

    // Verify signature
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)data.constData(), data.length(), hash);

    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, (const unsigned char *)signature.constData(), signature.length(), rsa);
    RSA_free(rsa);
    BIO_free(bio);

    return (result == 1);
}
