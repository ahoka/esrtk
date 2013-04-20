/* 
 * File:   Function.h
 * Author: edmmhka
 *
 * Created on August 29, 2012, 2:31 PM
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

class Function {
public:
	Function();
	virtual ~Function();
	
	virtual int attach() = 0;
	virtual int detach() = 0;
	virtual int probe() = 0;
private:
	Function(const Function& orig);
};

#endif	/* FUNCTION_H */

