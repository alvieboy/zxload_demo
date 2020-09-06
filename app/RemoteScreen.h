#include <QWidget>
#include <QTimer>
#include "qzeroconf.h"
#include <QTcpSocket>
#include <QUdpSocket>

#define MAX_FRAME_PAYLOAD 2048
#define SPECTRUM_FRAME_SIZE (32*(192+24))

typedef struct {
    uint8_t pixeldata[32*192];
    uint8_t attributes[32*24];
} __attribute__((packed)) scr_t;


class RemoteScreen: public QWidget
{
    Q_OBJECT
public:
    RemoteScreen();

    void start();
public slots:
    void timerExpired();
    void serviceAdded(QZeroConfService);
    void serviceUpdated(QZeroConfService);
    void serviceRemoved(QZeroConfService);
    void zeroConfError(QZeroConf::error_t);

    void controlSocketConnected();
    void controlSocketDisconnected();
    void controlSocketError(QAbstractSocket::SocketError socketError);
    void UdpDataAvailable();

    void renderScreen(const scr_t *scr, bool flash_only);
    void paintEvent(QPaintEvent *) override;
    
protected:

    enum state {
        STATE_SERVICE_DISCOVERY,
        STATE_CONNECT_SERVICE,
        STATE_CONTROL_CONNECTED,
        STATE_STREAM
    };
    void setControl(QHostAddress host, quint16 port);
    void startStream();
    void sendStreamCommand();
    void enterState(enum state st);
    void resetReceiverState();
    void UdpProcess(const uint8_t *data, qint64 len);

signals:
    void connectionDetected();
    void connectionLost();

private:

    QTimer timer;
    QZeroConf zeroconf;
    enum state state;

    QHostAddress control_host;
    quint16 control_port;
    QTcpSocket control_socket;

    QUdpSocket stream_socket;

    uint8_t framedata[SPECTRUM_FRAME_SIZE];
    QImage targetimage;
    bool flashinvert;
    bool isconnected;
};
