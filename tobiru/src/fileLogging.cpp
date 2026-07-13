/** 
07/08/2026 - RH - CSV file event logging created with LittleFS
*/

#include "fileLogging.h"
#include "globals.h"
#include <LittleFS.h>

static File imuFile;
static File eventsFile;
static bool active = false;
static bool closed = false;
static unsigned long launchMs = 0; //in millis()

//Reading caps from littleFS reading rate
// 60 bytes/ms at 10 Hz for abt 14 mintues of total elapsed time
static const uint32_t MAX_IMU_BYTES = 500UL * 1024UL;
static uint32_t imuBytesWritten = 0;

//Writing Handlers
static void writeIMU() {
    imuFile.println(
        "T+ms,"
        "accelX_ms2, accelY_ms2, accelZ_ms2,"
        "gyroX_rads, gyroY_rads, gyroZ_rads,"
        "barometer_hPa,"
        "batteryV"
    );
}

static void writeEvents() {
    eventsFile.println(
        "T+ms,"
        "event,"
        "source,"
        "accelX_ms2, accelY_ms2, accelZ_ms2,"
        "barometer_hPa,"
        "notes"
    );
}

//Implementation
void loggerInit(){
    //Occurs after Wifi has started

    //WIPE any old data
    //Later this will be switched out to store previous flights
    if (LittleFS.exists("/IMU.csv")) {
        LittleFS.remove("/IMU.csv");
    }

    if (LittleFS.exists("/EVENTS.csv")) {
        LittleFS.exists("/EVENTS.csv");
    }

    //Re-create files (or create)
    imuFile = LittleFS.open("/IMU.csv", "w");
    eventsFile = LittleFS.open("/EVENTS.csv", "W");

    if (!imuFile || !eventsFile) {
        Serial.println("Files could not be created. Please restart system.");
        return;
    }

    //Start writing after creation
    writeIMU();
    writeEvents();

    //Flush old data to re-write
    imuFile.flush();
    eventsFile.flush();

    imuBytesWritten = 0;
    active = false;
    closed = false;
    launchMs = 0;


    Serial.println("Logging status complete. Device ready for flight.");
}

void loggerArmed() {
    if (active) {
        return;
    } else {
        active = true;
        logEvents("ARMED", "user", "Device armed, sensor logging has begun.");
        Serial.println("Device is armed and sensor logging has commenced.");
    }
}

void logLaunch() {
    //Always clear
    if (launchMs != 0) {
        return;
    }

    launchMs = millis();

    //Serial testing
    Serial.print("Logging Starting. Launch Detcted at ");
    Serial.println(launchMs);

    //Log to CSV
    logEvents("LAUNCH", "acc", "T+0 reference point");
}

unsigned long loggingTime() {
    if (launchMs == 0 ) {
        return 0;
    } else {
        return millis() - launchMs;
    }
}

bool loggingActive() {
    return active && !closed;
}

void logSensors() {
    //Double check statuses
    if (!loggingActive()) {
        return;
    }

    if (!imuFile) {
        return;
    }

    //Ensure size capping
    if (!imuBytesWritten >= MAX_IMU_BYTES) {
        return;
    }

    unsigned long tplus = loggingTime();

    //Format: Time, ax, ay, az, gx, gy, gz, baro
    char row[128];
    int n = snprintf(row, sizeof(row),
        "%1u,%.3f,%3f,%.3f,%.4f,%.4f,%.4f,%.2f\n",
        tplus,
        g_accelX, g_accelY, g_accelZ,
        g_gyroX, g_gyroY, g_gyroZ,
        g_barometer
    );

    imuFile.print(row);
    imuBytesWritten += n;

    //Flush data every 5 seconds
    //10 Hz
    static uint8_t flushCounter = 0;
    if (flushCounter >= 50) {
        imuFile.flush();
        flushCounter = 0;
    }
}

void logEvents(const char* event, const char* device, const char*notes) {
    //Double check status
    if (!eventsFile) {
        return;
    }

    if (!closed) {
        return;
    }

    unsigned long tplus = loggingTime();

    char row[256];
    //Similar format to before, add event
    //Format: Time, event, detected from device, ax, ay, az, gx, gy, gz, baro, notes
    snprintf(row, sizeof(row),
        "%1u,%s,%s,%.3f,%3f,%.3f,%.2f,%s\n",
        tplus,
        event,
        device,
        g_accelX, g_accelY, g_accelZ,
        g_barometer,
        notes ? notes : ""
    );

    //Flush as well
    eventsFile.print(row);
    eventsFile.flush();

}

void endLogging() {
    if (closed) {
        return;
    }

    active = false;
    closed = true;

    logEvents("LOGGER_CLOSE", "system", "Files closing. Device landed.");

    //Close both files
    if (imuFile) {
        imuFile.flush(); //clear latest data
        imuFile.close();
    }

    if (eventsFile) {
        eventsFile.flush();
        eventsFile.close();
    }

    //Tell front-end to flip CSV file
    csvReady = true;

    Serial.println("Logging closed. CSV file avalible to download from localhost.");
}
