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
   Debug() = delete;
   Debug(const Debug& orig) = delete;
   ~Debug() = delete;

public:
   static void panic (const char*, ...) __attribute__((noreturn));
   
   static void verbose(const char*, ...);
   static void info(const char*, ...);
   static void warning(const char*, ...);
   static void error(const char*, ...);


   enum class DebugLevel
   {
      Silent,
      Error,
      Warning,
      Info,
      Verbose
   };

private:
   static void printTimeStamp();

   static DebugLevel debugLevel;
};

#endif	/* DEBUG_H */
