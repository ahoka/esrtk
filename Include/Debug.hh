/* 
 * File:   Debug.h
 * Author: edmmhka
 *
 * Created on August 17, 2012, 1:39 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#define KASSERT(x) if (!(x)) { Debug::panic("Assertion (" #x ") failed at " __FILE__ ":%d\n", __LINE__); }

class Debug {
private:
   Debug();
   Debug(const Debug& orig);
   ~Debug();
public:
   static void panic(const char*, ...);
};

#endif	/* DEBUG_H */
