# SATtiny

<p>The idea of this mini project is to make two LEDs blink in a predefine pattern that looks similar to an exponential function given by an RC circuit, creating a smooth fadding effect.</p>
<p>This is the code running on the SATtiny. After some measurements, it has been obesrved that the current consumption remains around 4uA wwhen sleeping. During the day, when there is enough light to load the super cap, the Attiny wakes up every 8 seconds to checkout if PB3 is still high. In abscense of light, PB3 sees a logical 0. At this point, the wake up interval changes to 4 seconds.</p>
<p>Have fun!</p>
