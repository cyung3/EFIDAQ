#include "udpreader.h"

#include <QAbstractSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTime>

UDPReader::UDPReader(QObject* parent)
    : QObject(parent)
{
    m_udpSocket = new QUdpSocket;

    connect(m_udpSocket, SIGNAL(readyRead()),
            this, SLOT(handleReadyRead()));
    connect(m_udpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(handleError(QAbstractSocket::SocketError)));

    m_isCollecting = false;
}

int UDPReader::availableData(QByteArray& data)
{
    data.append(m_data);
    int size = m_data.size();
    m_data.clear();
    return size;
}

void UDPReader::advertise() const
{

}

bool UDPReader::open()
{
    close();
    if (m_udpSocket->bind(m_IPAddress, m_portNo, QAbstractSocket::ShareAddress))
    {
       m_isCollecting = true;
       return true;
    }
    return false;
}

bool UDPReader::isOpen()
{
    return m_isCollecting;
}

bool UDPReader::close()
{
    m_isCollecting = false;
    m_udpSocket->close();
    return true;
}

QString UDPReader::getIPAddressString() const
{
    return m_IPAddress.toString();
}

quint16 UDPReader::getPortNo() const
{
    return m_portNo;
}

bool UDPReader::setIPAddress(QString newIP)
{
    QString oldIP = m_IPAddress.toString();
    if (!m_IPAddress.setAddress(newIP))
    {
        m_IPAddress.setAddress(oldIP);
        return false;
    }
    return true;
}

bool UDPReader::setPortNo(quint16 portNo)
{
    m_portNo = portNo;
    return true;
}

void UDPReader::handleReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray inBytes;
        inBytes.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(inBytes.data(), inBytes.size());
        if (m_isCollecting)
        {
            m_data.append(inBytes);
        }
    }
}

void UDPReader::handleError(QAbstractSocket::SocketError errnum)
{
    switch(errnum)
    {
    case QAbstractSocket::ConnectionRefusedError:
        break;
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        break;
    }
}



