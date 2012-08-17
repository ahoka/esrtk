/* 
 * File:   Power.h
 * Author: edmmhka
 *
 * Created on August 17, 2012, 1:39 PM
 */

#ifndef POWER_H
#define	POWER_H

class Power {
private:
	Power();
	Power(const Power& orig);
	virtual ~Power();
public:
	static void halt(const char*);
	static void reboot();
};

#endif	/* POWER_H */

