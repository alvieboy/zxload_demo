#include "RemoteScreen.h"
#include <QDebug>
#include <QPainter>

struct frame {
    uint8_t seq:7;
    uint8_t val:1;
    uint8_t frag;
    uint8_t payload[MAX_FRAME_PAYLOAD];
} __attribute__((packed));

RemoteScreen::RemoteScreen(): targetimage(160,128,QImage::Format_RGB32)
{
    QObject::connect(&zeroconf, &QZeroConf::serviceAdded, this, &RemoteScreen::serviceAdded);
    QObject::connect(&zeroconf, &QZeroConf::serviceRemoved, this, &RemoteScreen::serviceRemoved);
    QObject::connect(&zeroconf, &QZeroConf::serviceUpdated, this, &RemoteScreen::serviceUpdated);
    QObject::connect(&zeroconf, &QZeroConf::error, this, &RemoteScreen::zeroConfError);
    QObject::connect(&timer, &QTimer::timeout, this, &RemoteScreen::timerExpired);
    timer.setSingleShot(true);

    stream_socket.bind();
    QObject::connect(&stream_socket, &QIODevice::readyRead, this, &RemoteScreen::UdpDataAvailable);
    flashinvert = false;
    targetimage.fill(0x0);
    isconnected = false;
}

void RemoteScreen::serviceAdded(QZeroConfService service)
{
    qDebug()<<"Service added";

    if (state == STATE_SERVICE_DISCOVERY) {
        timer.stop();

        setControl( service->ip(), service->port() );

        enterState(STATE_CONNECT_SERVICE);
    }

}

void RemoteScreen::serviceUpdated(QZeroConfService)
{
    qDebug()<<"Service updated";
}

void RemoteScreen::serviceRemoved(QZeroConfService)
{
    qDebug()<<"Service removed";
}

void RemoteScreen::zeroConfError(QZeroConf::error_t error)
{
    qDebug()<<"Zeroconf error"<<error;
}

void RemoteScreen::start()
{
    enterState(STATE_SERVICE_DISCOVERY);
}

void RemoteScreen::enterState(enum RemoteScreen::state st)
{
    state = st;
    switch(state) {
    case STATE_SERVICE_DISCOVERY:
        qDebug()<<"Starting service discovery";
        zeroconf.startBrowser("_smb._tcp");
        //zeroconf.startBrowser("_zxictrl._tcp");
        timer.start(30000);
        break;
    case STATE_CONNECT_SERVICE:
        zeroconf.stopBrowser();
        qDebug()<<"Start "<<control_host<<control_port;
        startStream();
        break;
    case STATE_CONTROL_CONNECTED:
        sendStreamCommand();
        break;
    case STATE_STREAM:
        timer.start(10000);
    default:
        break;
    }
}

void RemoteScreen::sendStreamCommand()
{
    QString cmd = "stream ";

    cmd += QString::number( stream_socket.localPort() ) + "\n";
    resetReceiverState();

    control_socket.write(cmd.toLocal8Bit());
    control_socket.flush();
    control_socket.disconnect();
    control_socket.close();

    enterState(STATE_STREAM);
}

void RemoteScreen::timerExpired()
{
    qDebug()<<"Timer elapsed"<<zeroconf.browserExists();
    switch (state) {
    case STATE_SERVICE_DISCOVERY:
        enterState(STATE_SERVICE_DISCOVERY);
        break;
    case STATE_STREAM:
        qDebug()<<"Lost data";
        isconnected = false;
        emit connectionLost();
        enterState(STATE_SERVICE_DISCOVERY);
        break;
    default:
        break;
    }
}

void RemoteScreen::setControl(QHostAddress host, quint16 port)
{
    control_host = host;
    control_port = port;
}

void RemoteScreen::controlSocketConnected()
{
    qDebug()<<"Connected";
    enterState(STATE_CONTROL_CONNECTED);
}

void RemoteScreen::controlSocketDisconnected()
{
    qDebug()<<"Disconnected";
}

void RemoteScreen::controlSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"Error";
}

void RemoteScreen::startStream()
{
    control_socket.connectToHost( control_host, control_port );
    QObject::connect(&control_socket, &QAbstractSocket::connected, this, &RemoteScreen::controlSocketConnected );
    QObject::connect(&control_socket, &QAbstractSocket::disconnected, this, &RemoteScreen::controlSocketDisconnected );
    
    //QObject::connect(&control_socket, &QAbstractSocket::errorOccurred, this, &RemoteScreen::controlSocketError );


    //write(tcp_socket, "stream 8010\n",12);
    //shutdown(tcp_socket, SHUT_WR);

}

void RemoteScreen::UdpDataAvailable()
{
    uint8_t data[16368];
    qint64 r = stream_socket.read((char*)data, sizeof(data));

    if (r>0) {
        UdpProcess(data, r);
    }
}

void RemoteScreen::resetReceiverState()
{
}

void RemoteScreen::UdpProcess(const uint8_t *data, qint64 len)
{
    if (len<2)
        return;

    if (state != STATE_STREAM)
        return;

    unsigned payloadlen = len - 2;
    //qDebug()<<"Dgram";
    const struct frame *f = (const struct frame*)data;

    unsigned maxpayload = 1<<((f->frag) >> 4);

    uint8_t lastfrag = (((unsigned)SPECTRUM_FRAME_SIZE+((unsigned)maxpayload-1))/(unsigned)maxpayload)-1;
    uint8_t frag = f->frag & 0xf;
//    printf("UDP: seq %d frag %d lastfrag %d maxpayload %d\n", f->seq, frag, lastfrag,maxpayload);

    memcpy( &framedata[ frag * maxpayload ], f->payload, payloadlen);

    if (frag>=lastfrag) {
        renderScreen( (const scr_t*)framedata, false);

        if (!isconnected)
            emit connectionDetected();
        isconnected = true;

    }
}

/* Colors 1/4 of maximum value, so we can add them */
static const uint32_t normal_colors32_1_4[] = {
    0x00000000,
    0x00000036,
    0x00360000,
    0x00360036,
    0x00003600,
    0x00003636,
    0x00363600,
    0x00363636
};

static const uint32_t bright_colors32_1_4[] = {
    0x00000000,
    0x0000003F,
    0x003F0000,
    0x003F003F,
    0x00003F00,
    0x00003F3F,
    0x003F3F00,
    0x003F3F3F
};


static inline uint32_t parsecolor32( uint8_t col, int bright)
{
    return bright ? bright_colors32_1_4[col] : normal_colors32_1_4[col];
}

static inline void parseattr32( uint8_t attr, uint32_t *fg, uint32_t *bg, int *flash)
{
    *fg = parsecolor32(attr & 0x7, attr & 0x40);
    *bg = parsecolor32((attr>>3) & 0x7, attr & 0x40);
    *flash = attr & 0x80;
}

static inline uint8_t getattr(const scr_t *scr, int x, int y)
{
    return scr->attributes[ x + (y*32) ];
}

#define BORDER_LR 16
#define BORDER_TB 16

void RemoteScreen::renderScreen(const scr_t *scr, bool flashonly)
{
    // 2x2 pixelization.
    QPainter painter;

    int x;
    int y;
    uint32_t fg, bg, t;
    int flash;

    for (y=0;y<192;y+=2) {
        for (x=0; x<32; x++) {
            unsigned offset = x;

            offset |= ((y>>3) & 0x7)<<5;  // Y5, Y4, Y3
            offset |= (y & 7)<<8;         // Y2, Y1, Y0
            offset |= ((y>>6) &0x3 ) << 11;               // Y8, Y7


            unsigned offset_next = x;

            offset_next |= (((y+1)>>3) & 0x7)<<5;  // Y5, Y4, Y3
            offset_next |= ((y+1) & 7)<<8;         // Y2, Y1, Y0
            offset_next |= (((y+1)>>6) &0x3 ) << 11;               // Y8, Y7

            uint8_t attr = getattr(scr, x, y>>3);
            parseattr32( attr, &fg, &bg, &flash);

            if (!flash && flashonly)
                continue;

            if (flash && flashinvert) {
                t = bg;
                bg = fg;
                fg = t;
            }
            // Get m_scr
            uint8_t pixeldata8 = scr->pixeldata[offset];
            uint8_t pixeldata8_nl = scr->pixeldata[offset_next]; // Next line

            //printf("%d %d Pixel %02x attr %02x fg %08x bg %08x\n", x, y, pixeldata8, attr, fg, bg);

            for (int ix=0;ix<4;ix++) {
                // Parse 4 pixels at a time.
                uint32_t color = 0;

                if (pixeldata8 & 0x80)
                    color += fg;
                else
                    color += bg;

                if (pixeldata8 & 0x40)
                    color += fg;
                else
                    color += bg;

                if (pixeldata8_nl & 0x80)
                    color += fg;
                else
                    color += bg;
                if (pixeldata8_nl & 0x40)
                    color += fg;
                else
                    color += bg;

                targetimage.setPixel(BORDER_LR + (x*4 + (ix)),
                                     BORDER_TB+(y/2),
                                     color);
                pixeldata8    <<=2;
                pixeldata8_nl <<=2;
            }
        }
    }
    repaint();
};

void RemoteScreen::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawImage(0, 0, targetimage);//.scaled(size(),Qt::KeepAspectRatio,Qt::FastTransformation));


}
