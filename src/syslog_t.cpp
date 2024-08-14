#include "syslog_t.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Syslog.h>
#include "WebSettings.h"
#include "defines.h"

static const char* TAG = "SYSLOG";

// Syslog server connection info
//#define SYSLOG_SERVER "192.168.55.109"
//#define SYSLOG_PORT 514

// This device info
//#define DEVICE_HOSTNAME "my-device"
//#define APP_NAME "my-app"

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClient;
Syslog syslog(udpClient);

// Create a new syslog instance with LOG_KERN facility
//Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

static String str_mSyslogHostName;
static String str_mSyslogDeviceName;
static String str_mSyslogAppName;


//uint32_t u32_mSyslogPublishLoopTimmer=0;


enum enum_smSyslogEnableState {SYSLOG_ENABLE_STATE_OFF, SYSLOG_ENABLE_STATE_EN, SYSLOG_ENABLE_STATE_READY};
enum_smSyslogEnableState  mSyslogEnable = SYSLOG_ENABLE_STATE_OFF;

int iteration = 1;

void initSyslog()
{

  if(WebSettings::getBool(ID_PARAM_SYSLOG_SERVER_ENABLE,0)) mSyslogEnable = SYSLOG_ENABLE_STATE_EN;
  if(mSyslogEnable == SYSLOG_ENABLE_STATE_OFF) return;

  if(!WebSettings::getString(ID_PARAM_SYSLOG_SERVER_HOST,0).equals(""))
  {
	str_mSyslogHostName = WebSettings::getString(ID_PARAM_SYSLOG_SERVER_HOST,0);
  str_mSyslogDeviceName = WebSettings::getString(ID_PARAM_BSC_DEVICE_NAME,0);
  str_mSyslogAppName = WebSettings::getString(ID_PARAM_SYSLOG_APP_NAME,0);
	syslog.server(str_mSyslogHostName.c_str(),(uint16_t)WebSettings::getInt(ID_PARAM_SYSLOG_SERVER_PORT,0,DT_ID_PARAM_SYSLOG_SERVER_PORT));
  syslog.deviceHostname(str_mSyslogDeviceName.c_str());
  syslog.appName(str_mSyslogAppName.c_str());
	BSC_LOGI(TAG,"SYSLOG: host=%s, port=%i", str_mSyslogHostName.c_str(), WebSettings::getInt(ID_PARAM_SYSLOG_SERVER_PORT,0,DT_ID_PARAM_SYSLOG_SERVER_PORT));
  }
  mSyslogEnable = SYSLOG_ENABLE_STATE_READY;
}

bool syslogLoop()
{
	  // Severity levels can be found in Syslog.h. They are same like in Linux
  // syslog.
  syslog.log(LOG_INFO, "Begin loop");

  // Log message can be formated like with printf function.
  syslog.logf(LOG_ERR,  "This is error message no. %d", iteration);
  syslog.logf(LOG_INFO, "This is info message no. %d", iteration);

  // You can force set facility in pri parameter for this log message. More
  // facilities in syslog.h or in Linux syslog documentation.
  syslog.logf(LOG_DAEMON | LOG_INFO, "This is daemon info message no. %d",
    iteration);

  // F() macro is supported too
  syslog.log(LOG_INFO, F("End loop"));
  iteration++;

	return false;
}

bool sendSyslogf(const char *tag, const char *fmt, ...) {
  if(mSyslogEnable != SYSLOG_ENABLE_STATE_READY) return false;
  va_list args;
  bool result;

  va_start(args, fmt);
  result = syslog.vlogf(LOG_INFO, fmt,args);
  va_end(args);
  return result;
}

void sendSyslog()
{
	if(mSyslogEnable != SYSLOG_ENABLE_STATE_READY) return;
	syslog.log(LOG_INFO, "Begin loop");
}