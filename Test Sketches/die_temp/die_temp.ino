#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define DIE_TEMP_ALIAS(i) DT_ALIAS(_CONCAT(die_temp, i))
#define DIE_TEMPERATURE_SENSOR(i, _) \
  IF_ENABLED(DT_NODE_EXISTS(DIE_TEMP_ALIAS(i)), (DEVICE_DT_GET(DIE_TEMP_ALIAS(i)), ))

/* support up to 16 cpu die temperature sensors */
static const struct device *const sensors[] = { LISTIFY(16, DIE_TEMPERATURE_SENSOR, ()) };


static int get_die_temperature(const struct device *dev) {
  struct sensor_value val;
  int rc;

  //printk("get_die_temperature: %p\n", dev);
  /* fetch sensor samples */
  rc = sensor_sample_fetch(dev);
  //printk("sensor_sample_fetch: %d\n", rc);
  if (rc) {
    printk("Failed to fetch sample (%d)\n", rc);
    return rc;
  }

  rc = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &val);
  //printk("sensor_channel_get: %d\n", rc);
  if (rc) {
    printk("Failed to get data (%d)\n", rc);
    return rc;
  }

  printk("CPU Die temperature[%s]: %.1f °C\n", dev->name, sensor_value_to_double(&val));
  return 0;
}

double CPUTemperature(uint8_t sensor_index = 0) {
  if (sensor_index >= ARRAY_SIZE(sensors)) return 0.0 / 0.0;
  const struct device *dev = sensors[0];

  struct sensor_value val;
  int rc;

  //printk("get_die_temperature: %p\n", dev);
  /* fetch sensor samples */
  rc = sensor_sample_fetch(dev);
  //printk("sensor_sample_fetch: %d\n", rc);
  if (rc) return 0.0 / 0.0;

  rc = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &val);
  if (rc) return 0.0 / 0.0;

  return sensor_value_to_double(&val);
}

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 5000) {}
  delay(2000);
  Serial.print("Test DIE_TEMP count: ");
  Serial.println(ARRAY_SIZE(sensors));

  for (size_t i = 0; i < ARRAY_SIZE(sensors); i++) {
    int rc = get_die_temperature(sensors[i]);
    if (rc < 0) {
      return;
    }
  }
}

void loop() {
  double temp = CPUTemperature();
  Serial.print("CPU Temp C:");
  Serial.print(temp, 2);
  if (!isnan(temp)) {
    Serial.print(" F:");
    Serial.print(temp * 1.8 + 32.0, 2);
  }
  Serial.println();
  delay(5000);
}
