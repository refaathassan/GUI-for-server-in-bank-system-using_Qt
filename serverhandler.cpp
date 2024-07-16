#include "serverhandler.h"  // Include the header file for ServerHandler class

// Constructor for ServerHandler class, inheriting QThread
ServerHandler::ServerHandler(qint32 ID, QObject *parent)
    : QThread{parent},  // Call base class constructor with parent object
    ID{ID}  // Initialize member variable ID with provided ID
{
    // Instantiate all handler objects for processing requests
    PRU1 = new LogHandler();
    PRU2 = new GetAccounHandler();
    PRU3 = new ViewAccountBalanceHandler();
    PRU4 = new ViewDataBase();
    PRU5 = new DeleteHandler();
    PRU6 = new AddUserHandler();
    PRU7 = new MakeTransactionHandler();
    PRU8 = new TransferAmountHandler();
    PRU9 = new ViewTransactionHistoryHandler();
    PRU10 = new UpdateUserHandler();
    flag=false;
    // Set up handler chain
    PRU1->SetNextHandler(PRU2);
    PRU2->SetNextHandler(PRU3);
    PRU3->SetNextHandler(PRU4);
    PRU4->SetNextHandler(PRU5);
    PRU5->SetNextHandler(PRU6);
    PRU6->SetNextHandler(PRU7);
    PRU7->SetNextHandler(PRU8);
    PRU8->SetNextHandler(PRU9);
    PRU9->SetNextHandler(PRU10);

    key = "abcdefghijklmnop";  // Initialize AES encryption key
    iv = "1234567890123456";   // Initialize AES encryption IV
   // publick = QCoreApplication::applicationDirPath() + "//public.pem";  // Set path to RSA public key file
    //connect(PRU1,&Handler::SendToSocket,this,&ServerHandler::SendMassage);  // Example of signal-slot connection (commented out)
}

// Method called when the client disconnects
void ServerHandler::OnDisconnect()
{
    if(soc->isOpen())  // Check if socket is open
    {
        soc->close();  // Close the socket
        //qDebug()<<"client =>"<<ID<<"  Disconnected  "<<Qt::endl;  // Debug output indicating client disconnection
    }
}

// Method called when data is ready to be read from the socket
void ServerHandler::OnReadyRead()
{
    QByteArray B_arrary = soc->readAll();  // Read all available data from socket
    if(flag==false)
    {
        QJsonDocument json = QJsonDocument::fromJson(B_arrary);  // Parse decrypted data as JSON document
        QJsonObject jsonObj = json.object();  // Convert JSON document to JSON object
        publick=jsonObj["key"].toString();
        flag=true;
    }
    else
    {
        QByteArray dec_B_arrary = decryptAndVerify(B_arrary, key, iv, publick.toUtf8());  // Decrypt and verify received data
        QJsonDocument json = QJsonDocument::fromJson(dec_B_arrary);  // Parse decrypted data as JSON document
        QJsonObject jsonObj = json.object();  // Convert JSON document to JSON object
        Operation(jsonObj);  // Process the received JSON object
    }
}

// Method to send a message to the client
void ServerHandler::SendMassage(QJsonObject json)
{
    if(soc->isOpen())  // Check if socket is open
    {
        QByteArray byte = QJsonDocument(json).toJson(QJsonDocument::Compact);  // Convert JSON object to compact JSON byte array
        soc->write(byte);  // Write the byte array to the socket
    }
}

// Method to handle operations based on the received JSON object
void ServerHandler::Operation(QJsonObject json)
{
    QJsonObject js = PRU1->Handling(json);  // Handle the JSON object using the first handler in the chain
    SendMassage(js);  // Send the response back to the client
}

// Overridden run method of QThread, called when the thread starts
void ServerHandler::run()
{
    //qDebug()<<"client=>  "<<ID<<"   is running in thread=> "<<QThread::currentThreadId()<<Qt::endl;  // Debug output indicating thread and client ID
    soc = new QTcpSocket;  // Create a new TCP socket
    soc->setSocketDescriptor(ID);  // Set socket descriptor for the created socket
    connect(soc, &QTcpSocket::readyRead, this, &ServerHandler::OnReadyRead, Qt::DirectConnection);  // Connect readyRead signal of socket to OnReadyRead slot
    connect(soc, &QTcpSocket::disconnected, this, &ServerHandler::OnDisconnect, Qt::DirectConnection);  // Connect disconnected signal of socket to OnDisconnect slot
    //SendMassage("Hello From My Server...........");  // Example message sent upon client connection (commented out)
    exec();  // Enter thread's event loop
}

// Method to decrypt and verify the received data using AES and RSA
QByteArray ServerHandler::decryptAndVerify(const QByteArray &encryptedData, const QByteArray &key, const QByteArray &iv, const QByteArray &rsaPublicKeyFile)
{
    // Decrypt the base64 encoded data
    QByteArray encrypted = QByteArray::fromBase64(encryptedData);

    // Decrypt the data using AES
    QByteArray decrypted = QAESEncryption::Decrypt(QAESEncryption::AES_128, QAESEncryption::CBC, encrypted, key, iv, QAESEncryption::PKCS7);
    int padLength = decrypted.at(decrypted.length() - 1);  // Get the padding length
    decrypted = decrypted.left(decrypted.length() - padLength);  // Remove the padding from decrypted data

    int SIGNATURE_LENGTH = 256;  // Define the length of the RSA signature

    // Ensure decrypted data is at least as long as the RSA signature
    if (decrypted.length() <= SIGNATURE_LENGTH)
    {
        qDebug() << "Invalid decrypted data length";
        return QByteArray();
    }

    // Extract original data and received signature
    QByteArray originalData = decrypted.left(decrypted.length() - SIGNATURE_LENGTH);
    QByteArray receivedSignature = decrypted.right(SIGNATURE_LENGTH);

    // Verify the RSA signature
    if (!verifySignature(originalData, receivedSignature, rsaPublicKeyFile))
    {
        qDebug() << "Signature verification failed!";
        return QByteArray();  // Or handle the verification failure accordingly
    }

    return originalData;  // Return the original data if decryption and verification are successful
}

// Method to verify RSA signature
bool ServerHandler::verifySignature(const QByteArray &data, const QByteArray &signature, const QByteArray &rsaPublicKeyFile)
{
    // QFile file(rsaPublicKeyFile);  // Open the RSA public key file
    // if (!file.open(QIODevice::ReadOnly))
    // {
    //     qDebug() << "Failed to open public key file";
    //     return false;
    // }

    // QByteArray publicKeyData = file.readAll();  // Read all data from public key file
    // file.close();  // Close the file
    QByteArray publicKeyData =rsaPublicKeyFile;
    const char *publicKey = publicKeyData.constData();  // Get const char pointer to public key data

    // Read RSA public key using OpenSSL BIO
    BIO *bio = BIO_new_mem_buf((void*)publicKey, -1);
    if (!bio)
    {
        qDebug() << "Failed to create BIO for public key";
        return false;
    }

    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);  // Read RSA public key from BIO
    if (!rsa)
    {
        qDebug() << "Failed to load RSA public key";
        BIO_free(bio);  // Free the BIO memory
        return false;
    }

    // Calculate SHA-256 hash of the data
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)data.constData(), data.length(), hash);

    // Verify the RSA signature
    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, (const unsigned char *)signature.constData(), signature.length(), rsa);
    RSA_free(rsa);  // Free RSA structure
    BIO_free(bio);  // Free BIO memory

    return (result == 1);  // Return true if signature verification is successful
}
