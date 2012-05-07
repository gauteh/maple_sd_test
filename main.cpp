/* 2012-05-07: SD test for Olimexino STM32 H103 */

# include "wirish.h"

using namespace SdTest;


/* Force init to be called *first*, i.e. before static object allocation.
 * Otherwise, statically allocated objects that need libmaple may fail.   */
__attribute__((constructor)) void premain() {
  init();
}

int main(void) {
  SerialUSB.print ("SD Test for Olimexino v");
  SerialUSB.println (GIT_DESC);

  return 0;
}

/* vim: set filetype=arduino :  */

