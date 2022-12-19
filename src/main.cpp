#include <Arduino.h>
#include "Firmware.h"
#include "PN5180_Firmware.h"

#define REQ_PIN 13
#define NSS_PIN 21
#define RST_PIN 22
#define BUSY_PIN 33

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

PN5180_Firmware Pn5180(RST_PIN, BUSY_PIN, REQ_PIN, NSS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);

void setup() {
  Serial.begin(115200L);

  Pn5180.Begin();
}

void dumpInformation() {
  Serial.println("Press any key to dump PN5180 information...");
  Serial.flush();

  while (!Serial.available());
  while (Serial.available()) {
    Serial.read();
  }
  Serial.flush();

  Pn5180.End();
  Serial.end();
  Serial.begin(115200L);

  Pn5180.Begin();

  uint8_t MajorVersion, MinorVersion;
  uint8_t* DieId = (uint8_t*)malloc(PN5180_DL_GETDIEID_DIEID_LEN * sizeof(uint8_t));

  Pn5180.GetFirmwareVersion(&MajorVersion, &MinorVersion);
  Serial.print("[Info] Firmware Version ");
  Serial.print(MajorVersion, HEX);
  Serial.print(".");
  Serial.println(MinorVersion, HEX);
  
  Pn5180.GetDieID(DieId);
  Serial.print("[Info] DieID : ");
  Pn5180.PrintHex8(DieId, PN5180_DL_GETDIEID_DIEID_LEN);
  free(DieId);

  PN5180_DOWNLOAD_INTEGRITY_INFO_T IntegrityInfo;
  Pn5180.CheckIntegrity(&IntegrityInfo);
  Serial.print("[Info] Integrity -> FunctionCodeOk: ");
  Serial.println(IntegrityInfo.FunctionCodeOk);

  Serial.print("[Info] Integrity -> PatchCodeOk: ");
  Serial.println(IntegrityInfo.PatchCodeOk);

  Serial.print("[Info] Integrity -> PatchTableOk: ");
  Serial.println(IntegrityInfo.PatchTableOk);

  Serial.print("[Info] Integrity -> UserDataOk: ");
  Serial.println(IntegrityInfo.UserDataOk);

  PN5180_DOWNLOAD_SESSION_STATE_INFO SessionStateInfo;
  Pn5180.CheckSessionState(&SessionStateInfo);
  Serial.print("[Info] SessionState -> LifeCycle: ");
  Serial.println(SessionStateInfo.LifeCycle);

  Serial.print("[Info] SessionState -> SessionState: ");
  Serial.println(SessionStateInfo.SessionState);
}

void updateFirmware() {
  Serial.println("Press any key to update PN5180 firmware...");
  Serial.flush();

  while (!Serial.available());
  while (Serial.available()) {
    Serial.read();
  }
  Serial.flush();

  Pn5180.End();
  Serial.end();
  Serial.begin(115200L);
  Pn5180.Begin();

  if (Pn5180.DoUpdateFirmware((uint8_t*) PN5180_FIRMWARE, PN5180_FIRMWARE_SIZE)) {
    Serial.println("[Info] Update Success.\n");
  } else {
    Serial.println("[Error] Update Failed.\n");
  }
}


void loop() {
  dumpInformation();
  Serial.println("##########################################################");
  updateFirmware();
  Serial.println("##########################################################");
  dumpInformation();
  Serial.println("##########################################################");
}