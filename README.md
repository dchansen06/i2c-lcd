# i2c-lcd
A heavily extended version of [Scott Campbell](https://www.circuitbasics.com/raspberry-pi-i2c-lcd-set-up-and-programming/)'s version of [DenisFromHR](https://gist.github.com/DenisFromHR/cc863375a6e19dce359d)'s version by [Michael Horne](https://www.recantha.co.uk/blog/?p=4849) for use on the Raspberry Pi. Use at your own risk and discretion, and be careful to not fry your computer (watch the wires you plug in)!

## Getting started
See [Scott Campbell\'s guide](https://www.circuitbasics.com/raspberry-pi-i2c-lcd-set-up-and-programming/) for how to configure `I2C_LCD_driver.py` and how to setup the Raspberry Pi

## License
I am not sure what this code should be licensed as, it was inspired by a lot of people. I am not a lawyer, do not take this as legal advice. Please only use if you are allowed to.

## Heads-up
Do not let this run to long, I recommend setting up a [cron(8)](https://www.man7.org/linux/man-pages/man8/cron.8.html) job so that it turns off every so often... or else you WILL burn out your device

## systemD
Should you want to allow controlling the file with the various C programs, setup the lcd.service job unit file with the correct path and move it to `/etc/systemd/system/lcd.service`

See [systemd](https://wiki.debian.org/systemd) or [systemd(1)](https://man7.org/linux/man-pages/man1/systemd.1.html) for more information
