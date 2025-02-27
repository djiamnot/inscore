#include "MainWindow.h"

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include <QPushButton>
#include <QSettings>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QTimer>

#include <iostream>

#define DEFAULT_ADDRESS "127.0.0.1"
//#define DEFAULT_ADDRESS "marcopolo.grame.fr"
#define DEFAULT_PORT 7000

#define OUTPUT_BUFFER_SIZE 16384

#define DEBUG

//------------------------------------------------------------------------
void OSCMessage::send( const std::string& str , int port ) const
{
	UdpTransmitSocket transmitSocket( IpEndpointName( str.c_str() , port ) );
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	p << osc::BeginMessage( mAddress.c_str() ) << mCommand.c_str();
	for ( int i = 0 ; i < mValues.size() ; i++ )
	{
		QVariant::Type t = mValues[i].type();
		if ( t == QVariant::Int )
			p << int(mValues[i].toInt());
		else if ( t == QVariant::Double )
			p << float(mValues[i].toDouble());
		else
			p << mValues[i].toString().toUtf8().data();
	}
	p << osc::EndMessage;
    transmitSocket.Send( p.Data(), p.Size() );

}

//------------------------------------------------------------------------
ControllerWidget::ControllerWidget(QWidget *parent)
	 : QWidget(parent)
{
	setupUi(this);
	
	mRotateAllTimer = new QTimer(this);
	mRotateAllTimer->setInterval( 20 );
    connect(mRotateAllTimer, SIGNAL(timeout()), this, SLOT(rotateAll()));
	connect( mRotateAll , SIGNAL(toggled(bool)) , this , SLOT(rotateAllToggled(bool)) );
	
	connect( mQuit , SIGNAL(clicked()) , this , SLOT(ITLQuit()) );
	connect( mReset , SIGNAL(clicked()) , this , SLOT(ITLReset()) );
	connect( mScene1 , SIGNAL(clicked()) , this , SLOT(scene1()) );
}

//------------------------------------------------------------------------
void ControllerWidget::send( const OSCMessage& msg ) const
{
	msg.send( DEFAULT_ADDRESS, DEFAULT_PORT );
}

//------------------------------------------------------------------------
void ControllerWidget::ITLQuit()
{
	send( OSCMessage("/ITL").setCommand("quit") );
}

//------------------------------------------------------------------------
void ControllerWidget::ITLReset()
{
	send( OSCMessage("/ITL/scene").setCommand("reset") );
}

//------------------------------------------------------------------------
void ControllerWidget::scene1()
{
	int size = mScene1Int->value();
	if (!size) return;

	float fsize = float(size);
	const float base = 1.5;
	float rsize = base / fsize;
	float step = rsize + 0.5/fsize;
	float x = 0 - (step * (size-1))/2;
	float x0 =x, y = x;
	for ( int i = 0 ; i < size ; i++ )
	{
		for ( int j = 0 ; j < size ; j++ )
		{
			std::string rectName = QString( "/ITL/scene/rect" + QVariant( i*size + j ).toString() ).toStdString();
			send( OSCMessage( rectName ).setCommand("set").addString("rect").addFloat(rsize).addFloat(rsize) );
			send( OSCMessage( rectName ).setCommand("x").addFloat( x ) );
			send( OSCMessage( rectName ).setCommand("y").addFloat( y ) );
			x += step;
		}
		x = x0;
		y += step;
	}	
}

//------------------------------------------------------------------------
void ControllerWidget::rotateAll()
{
	send( OSCMessage( QString("/ITL/scene/" + fRotateEdit->text()).toUtf8().data()  ).setCommand("dangle").addFloat( 1 ) );
}

//------------------------------------------------------------------------
void ControllerWidget::rotateAllToggled(bool toggled)
{
	if ( toggled )
		mRotateAllTimer->start();
	else
		mRotateAllTimer->stop();
}

//------------------------------------------------------------------------
MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags ) 
	:	QMainWindow ( parent , flags )
{
	QCoreApplication::setOrganizationName("GRAME");
    QCoreApplication::setOrganizationDomain("QtController");
	
	QSettings settings;
	resize(settings.value("size", QSize(100, 100)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	 
	ControllerWidget * centralW = new ControllerWidget(this);
	setCentralWidget(centralW);
}

//------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	QSettings settings;

    settings.setValue("size", size());
    settings.setValue("pos", pos());
}
