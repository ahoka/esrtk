/* 
 * File:   DriverManager.h
 * Author: edmmhka
 *
 * Created on August 29, 2012, 2:48 PM
 */

#ifndef DRIVERMANAGER_H
#define	DRIVERMANAGER_H

class Driver;

class DriverManager {
public:
	DriverManager();
	virtual ~DriverManager();
	
	//int registerDriver(Driver *driver);
	//int unregisterDriver(Driver* driver);
	
	//void probeAndAttach();
private:
	DriverManager(const DriverManager& orig);
};

#endif	/* DRIVERMANAGER_H */

