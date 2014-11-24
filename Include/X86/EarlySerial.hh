#ifndef EARLYSERIAL_H
#define	EARLYSERIAL_H

class EarlySerial {
public:
   static void init();
   static int putChar(int ch);
   static void disable();
   static bool isDisabled();

private:
   EarlySerial();
   virtual ~EarlySerial();
   EarlySerial(const EarlySerial& orig);
   EarlySerial& operator=(const EarlySerial& orig);

   static bool portInitialized;
   static bool isDisabledM;
};

#endif	/* EARLYSERIAL_H */
