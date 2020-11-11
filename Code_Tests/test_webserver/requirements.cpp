#include "requirements.h"
#include "Arduino.h"
#include <SD.h>
#include <FS.h>

int headerSize;


void clearSD(void) {
  Serial.println("Remove every .wav from SD");
  fs::FS fs = SD;
  
  fs::File root = SD.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }
  fs::File file = root.openNextFile();
  while (file) {
    String fileName = '/' + file.name();

    if (fileName != "interface.html" && fileName != "style.css") {
      if (fs.remove(fileName)) {
        Serial.println("File deleted");
      } else {
        Serial.println("Delete failed");
      }
    }
    file = root.openNextFile();
  }
}

String getUsedSpace(void) {

  int sumFileSize = 0;

  fs::File root = SD.open("/");
  if (!root) {
    Serial.println(F("Failed to open directory"));
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
  }

  fs::File file = root.openNextFile();
  while (file) {
    sumFileSize += file.size();
    file = root.openNextFile();
  }
  delay(1000);
  sumFileSize /= 1000;

  return String(sumFileSize);
}

void wavHeader(byte* header, int wavSize) {
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSize =  wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;  // linear PCM
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;  // linear PCM
  header[21] = 0x00;
  header[22] = 0x01;  // monoral
  header[23] = 0x00;
  header[24] = 0x80;  // sampling rate 44100
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;  // Byte/sec = 44100x2x1 = 88200
  header[29] = 0x7D;
  header[30] = 0x01;
  header[31] = 0x00;
  header[32] = 0x02;  // 16bit monoral
  header[33] = 0x00;
  header[34] = 0x10;  // 16bit
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
}
