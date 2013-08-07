//Comments...
#include <iostream>
using namespace std;
#include <ios>
#include <iomanip>

#include <linux/types.h>
#include <unistd.h>	//for usleep

#include "ADXL345.h"
#include "BMP085.h"
#include "HMC5883L.h"
#include "L3G4200D.h"

int main(){
	
	ADXL345 *myAccel=new(ADXL345);
	HMC5883L *myMag=new(HMC5883L);
	BMP085 *myBaro=new(BMP085);
	L3G4200D *myGyro=new(L3G4200D);

	myAccel->initialize();
	myGyro->initialize();
	myMag->initialize();
	myBaro->initialize();

	if (myAccel->testConnection())
			cout << "Connection to ADXL345 is working fine!" << endl;
		else
			cerr << "ERROR: No connection to ADXL345." <<endl;

	if (myGyro->testConnection())
			cout << "Connection to L3G4200D is working fine!" << endl;
		else
			cerr << "ERROR: No connection to L3G4200D." <<endl;

	if (myMag->testConnection())
			cout << "Connection to HMC5883L is working fine!" << endl;
		else
			cerr << "ERROR: No connection to HMC5883L." <<endl;

	if (myBaro->testConnection())
			cout << "Connection to BMP085 is working fine!" << endl;
		else
			cerr << "ERROR: No connection to BMP085." <<endl;


	myGyro->setFullScale(L3G4200D_FS_2000);

	myBaro->loadCalibration();

	int16_t ax, ay, az;
	int16_t hx, hy, hz;
	int16_t gx, gy, gz;

	float temp, p;

	// Baro: temp. measurerment once every second for correct pressure readout (according to datasheet).
	myBaro->setControl(BMP085_MODE_TEMPERATURE);
	usleep(4500);	//time to wait before measuring temperature (according to datasheet)
	temp=myBaro->getTemperatureC();
	cout << "temperature  = " << temp << "°C" << endl;

	// Baro: Return to normal pressure mode.
	myBaro->setControl(BMP085_MODE_PRESSURE_1);

	int N = 5;
	if (N>0)
		cout << "Performing "<<N<< " measurements..." << endl;
	for (int i=0;i<N;i++)
	{
		usleep(10000);
		myAccel->getAcceleration(&ax, &ay, &az);
		myGyro->getAngularVelocity(&gx, &gy, &gz);
		myMag->getHeading(&hx, &hy, &hz);

		p=myBaro->getPressure();

		cout << "Measurement " << i+1<< " of " << N << endl;

		cout << "acceleration = (" << ax << "; "<< ay << "; " << az << ") " << endl;
		cout << "angular rate = (" << gx << "; "<< gy << "; " << gz << ") " << endl;
		cout << "heading      = (" << hx << "; "<< hy << "; " << hz << ") " << endl;
		cout << "pressure     = " << p << " Pa"  << endl;

	}

	return 0;

}
