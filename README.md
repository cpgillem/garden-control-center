# Garden Control Center
## Garden Control Center
This is a basic Arduino sketch that watches for HTTP requests to turn on/off a solenoid valve.

* `POST` to `/region/#`: Turn on watering for a region.
* `DELETE` to `/region/#`: Turn off watering for a region.

### To Be Implemented

* Soil moisture sensors: Turn on watering when the soil moisture reaches a certain threshold.
* Multiple regions for different watering levels.
* Logging: Send requests to a server to save data, e.g. when watering is turned on/off, periodic soil moisture updates.
* Sensing for various environmental factors, e.g. temperature, sunlight, humidity.

## Garden Server

Not needed in the first iteration. Cron jobs can be used to control watering with simple HTTP requests.

### To Be Implemented

* More intelligent control over the Arduino than with curl/cron.
* Recording data from the Arduino.
* Serving a webpage for adjustments and monitoring.
