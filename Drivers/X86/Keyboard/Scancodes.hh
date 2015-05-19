class Scancodes
{
public:
   enum CodeType
   {
      Regular,
      Special,
   };

   struct Codes
   {
      uint8_t code;
      int ascii;
      int altascii;
      CodeType type;
   };

   enum Special
   {
      Error = 0x0100,
      Escape = 0x0101,
      Backspace = 0x0102,
      Tab = 0x0103,
      Enter = 0x0104,
      Control = 0x0105,
      LeftShift = 0x0106,
      RightShift = 0x0107,
      PrintScreen = 0x0108,
      Alt = 0x0109,
      Space = 0x010a,
      CapsLock = 0x010b,
      F1 = 0x010c,
      F2 = 0x010d,
      F3 = 0x010e,
      F4 = 0x010f,
      F5 = 0x0110,
      F6 = 0x0111,
      F7 = 0x0112,
      F8 = 0x0113,
      F9 = 0x0115,
      F10 = 0x0116,
      NumLock = 0x0117,
      ScrollLock = 0x0118,
      Home = 0x0119,
      UpArrow = 0x011a,
      Unused = 0xffff
   };

   static const Codes codes[];
};
