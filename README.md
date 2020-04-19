# SATtiny

<p align="center">
  <img width="300" src="http://rsflightronics.com/wp-content/uploads/2020/04/20200419_144652.jpg" alt="SATtiny Attiny25V">
</p>

<p>The idea of this mini project is to make two LEDs blink in a predefined pattern that looks similar to an exponential function given by an RC circuit, creating a smooth fading effect.</p>
<p>This is the code running on the SATtiny. After some measurements, it has been observed that the current consumption remains around 4uA when sleeping. During the day, when there is enough light to load the super cap, the Attiny wakes up every 8 seconds to checkout if PB3 is still high. In absence of light, PB3 sees a logical 0. At this point, the wake up interval changes to 4 seconds.</p>

<h2>SATtiny schematic</h2>

<p align="center">
  <img src="http://rsflightronics.com/wp-content/uploads/2020/04/SATtiny_SCH.png" alt="SATtiny schematic">
</p>

<p>&nbsp;</p>

<p>Have fun!</p>

<p align="center">
  <img width="400" src="http://rsflightronics.com/wp-content/uploads/2020/04/PSX_20200419_091607.jpg" >
</p>
