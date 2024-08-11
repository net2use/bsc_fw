#ifndef SYSLOG_T_H
#define SYSLOG_T_H

//#include <Arduino.h>



//Syslog syslog();
void initSyslog();
//bool syslogLoop();
void sendSyslog();
bool sendSyslogf(const char *tag, const char *fmt, ...);

#endif