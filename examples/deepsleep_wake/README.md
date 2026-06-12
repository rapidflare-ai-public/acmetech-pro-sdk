# deepsleep_wake

(Fictional demo content; see NOTICE.md.)

Sleeps in deep sleep and wakes every 10 seconds on the wake-up timer (WKT),
or when BUTTON0 is pressed. Each wake blinks LED0 and prints the time.

The HAL tick stops in deep sleep, so the printed time only counts awake time.
