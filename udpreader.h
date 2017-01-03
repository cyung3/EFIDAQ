#ifndef UDPREADER_H
#define UDPREADER_H

#include <QByteArray>
#include <QObject>
#include <QAbstractSocket>
#include <QHostAddress>

class QUdpSocket;

class UDPReader : public QObject
{
    Q_OBJECT

public:
    UDPReader(QObject* parent = nullptr);
    int availableData(QByteArray& data);
    bool open();
    bool isOpen();
    bool close();
    QString getIPAddressString() const;
    quint16 getPortNo() const;
    bool setIPAddress(QString newIP);
    bool setPortNo(quint16 portNo);

private:
    QByteArray m_data;
    QHostAddress m_IPAddress;
    quint16 m_portNo;

    QUdpSocket* m_udpSocket;

    bool m_isCollecting;

public slots:

private slots:
    void handleReadyRead();
    void handleError(QAbstractSocket::SocketError error);

signals:
    void stopCollecting();

};

#endif // UDPREADER_H
