/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QLineEdit>
#include <QtNetwork>
#include <QtGlobal>

#include "sender.h"

Sender::Sender(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Ready to broadcast datagrams."));
    statusLabel->setWordWrap(true);

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));
    lineEdit = new QLineEdit("0.0.0.0");

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    messageNo = 1;

    connect(startButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(stopBroadcasting()));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(changeAddress()));
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(lineEdit);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));
    setGeometry(500, 500, 250, 250);

    m_time.start();
    qsrand(0);
}

void Sender::startBroadcasting()
{
    startButton->setEnabled(false);
    timer->start(1000);
}

void Sender::stopBroadcasting()
{
    startButton->setEnabled(true);
    timer->stop();
}

void Sender::changeAddress()
{
    if (!m_IPAddress.setAddress(lineEdit->text()))
    {
        QMessageBox msgbox;
        msgbox.setText("Invalid Address.");
        msgbox.exec();
    }
}

void Sender::broadcastDatagram()
{
    statusLabel->setText(tr("Now broadcasting datagram %1").arg(messageNo));
    QByteArray datagram(
                (QString("%1,%2,%3,%4,%5,%6,%7\n")
                .arg(TIME()).arg(ECT()).
                arg(IAT()).arg(MAP()).
                arg(TPS()).arg(OIN()).arg(RPM())).toStdString().data());
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             m_IPAddress, 50000);
    ++messageNo;
}

double Sender::TIME()
{
   return m_time.elapsed() / 1000.0;
}

double Sender::ECT()
{
    return (qrand() % 20000) / 100.0;
}

double Sender::IAT()
{
    return (qrand() % 10000) / 100.0;
}

double Sender::MAP()
{
    return (qrand() % 10200) / 100.0;
}

double Sender::TPS()
{
    return sin(m_time.elapsed() / (200.0));
}

double Sender::OIN()
{
    return (qrand() % 900) / 100.0 + 7;
}

double Sender::RPM()
{
    return qrand() % 8000;
}
