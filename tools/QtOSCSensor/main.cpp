
#include <QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QObject>
#include <QDebug>


#include "Sensors.h"

class SensorAppl : public QApplication
{
	QQuickView	fView;
	int			fTimerID;
	Sensors		fSensors;
	public:
				 SensorAppl(int& argc, char** argv) : QApplication(argc, argv) {}
		virtual ~SensorAppl() {}
	
		void start();
	
	protected:
        void timerEvent(QTimerEvent * e);
};

void SensorAppl::start()
{
    fView.setSource(QUrl("qrc:/qml/wait.qml"));
    fView.show();
    fView.rootContext()->setContextProperty("sensors", &fSensors);
	connect((QObject*)fView.engine(), SIGNAL(quit()), this, SLOT(quit()));
	fTimerID = startTimer(2000);
}

void SensorAppl::timerEvent(QTimerEvent*)
{
	if (fSensors.network())
		fView.setSource(QUrl("qrc:/qml/SensorUI.qml"));
	else
		fView.setSource(QUrl("qrc:/qml/error.qml"));
	killTimer(fTimerID);
	fSensors.start((QObject*)fView.rootObject());
}


int main(int argc, char* argv[])
{
	SensorAppl app( argc , argv );
	app.start();
	return app.exec();
}
