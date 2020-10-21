#include "stateMachines.h"
#include "seg7_display.h"
void hwTimerISR(void *CallbackRef)
{
	updateDisplay();
	displayDigit();
}
