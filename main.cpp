/* 2012-05-07: SD test for Olimexino STM32 H103 */

# include "wirish.h"

/* Force init to be called *first*, i.e. before static object allocation.
 * Otherwise, statically allocated objects that need libmaple may fail.   */
__attribute__((constructor)) void premain() {
  init();
}

# include "HardwareSPI.h"
# include "SdFat.h"

int main(void) {
  for (int i = 0; i < 3; i++) {
    SerialUSB.print ("Starting soon: ");
    SerialUSB.println (i);
    delay(1000);
  }

  bool SD_AVAILABLE = false;

  SerialUSB.print ("SD Test for Olimexino v");
  SerialUSB.println (GIT_DESC);

  SerialUSB.println ("Initializing SD card..");
  Sd2Card  *card    = new Sd2Card ();
  SdVolume *volume  = new SdVolume ();
  SdFile   *root    = new SdFile ();

/* SPI 2
 *
 * NSS  = 25
 * SCK  = 32
 * MISO = 33
 * MOSI = 34
 */
# define SD_SPI 2
# define SD_CS  25 // chip select pin
  pinMode (SD_CS, OUTPUT);
  digitalWrite (SD_CS, HIGH);

  HardwareSPI *spi = new HardwareSPI (SD_SPI);

  /* Initialize SD card */
  spi->begin (SPI_281_250KHZ, MSBFIRST, 0);
  SD_AVAILABLE = card->init (spi, SD_CS);

  /*
  if (SD_AVAILABLE)
    SD_AVAILABLE = card->cardSize() > 0;
  */

  if (SD_AVAILABLE)
    SD_AVAILABLE = (card->errorCode () == 0);

  /* Beef up SPI speed after init is finished */
  if (SD_AVAILABLE)
    spi->begin (SPI_4_5MHZ, MSBFIRST, 0);

  /* Initialize FAT volume */
  if (SD_AVAILABLE)
    SD_AVAILABLE = volume->init (card, 1);

  /* Open root directory */
  if (SD_AVAILABLE)
    SD_AVAILABLE = root->openRoot (volume);

  if (SD_AVAILABLE)
  {
    SerialUSB.println ("[SD] Card initialized.");

    delay(1000);
    root->ls (LS_R | LS_DATE | LS_SIZE);

  } else {
    SerialUSB.println ("[SD] Could not initialize SD card.");
  }

  while (1) {
    delay(1000);
    SerialUSB.println ("Iteration");
  }
  return 0;
}

/* vim: set filetype=arduino :  */

