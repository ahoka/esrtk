/* 
 * File:   Supervisor.h
 * Author: edmmhka
 *
 * Created on August 29, 2012, 2:57 PM
 */

#ifndef SUPERVISOR_H
#define	SUPERVISOR_H

//#include <Drivers/Foundation/DriverManager.h>

class Supervisor {
public:
	Supervisor();
	virtual ~Supervisor();

	void run();

	//	DriverManager driverManager;
	// ...
private:
	Supervisor(const Supervisor& orig);
};

#endif	/* SUPERVISOR_H */

