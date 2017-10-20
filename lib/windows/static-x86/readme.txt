When you use uFCoder static library, you must define DL_USE_STATIC_LIB macro before include uFCoder.h
Also you must setup linking FTDI d2xx library in your project. FTDI distribute their library on its website http://www.ftdichip.com/Drivers/D2XX.htm
They stated in ftd2xx.h:
// Static D2XX depends on these Windows libs:
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
