/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <math.h>
#include <sys/types.h>

#include <utils/Errors.h>

#include <hardware/sensors.h>

#include "TemperatureSensor.h"
#include "SensorDevice.h"

namespace android {
// ---------------------------------------------------------------------------

int TemperatureSensor::getAlternateSensorType() const {
    if (mTemp.getType() == SENSOR_TYPE_TEMPERATURE) {
      return SENSOR_TYPE_AMBIENT_TEMPERATURE;
    } else {
      return SENSOR_TYPE_TEMPERATURE;
    }
}

TemperatureSensor::TemperatureSensor(const sensor_t& temp)
    : mSensorDevice(SensorDevice::getInstance()), mTemp(&temp)
{
}

bool TemperatureSensor::process(sensors_event_t* outEvent,
        const sensors_event_t& event)
{
    switch (event.type) {
        case SENSOR_TYPE_TEMPERATURE:
        case SENSOR_TYPE_AMBIENT_TEMPERATURE:
          *outEvent = event;
          outEvent->sensor = '_tmp';
          outEvent->type = getAlternateSensorType();
          return true;
        default:
            return false;
    }
}

status_t TemperatureSensor::activate(void* ident, bool enabled) {
    return mSensorDevice.activate(this, mTemp.getHandle(), enabled);
}

status_t TemperatureSensor::setDelay(void* ident, int handle, int64_t ns) {
    return mSensorDevice.setDelay(this, mTemp.getHandle(), ns);
}

Sensor TemperatureSensor::getSensor() const {
    sensor_t hwSensor;

    hwSensor.name       = mTemp.getName();
    hwSensor.vendor     = mTemp.getVendor();
    hwSensor.version    = mTemp.getVersion();
    hwSensor.handle     = '_tmp';
    hwSensor.type       = getAlternateSensorType();
    hwSensor.maxRange   = mTemp.getMaxValue();
    hwSensor.resolution = mTemp.getResolution();
    hwSensor.power      = mTemp.getPowerUsage();
    hwSensor.minDelay   = mTemp.getMinDelay();

    return Sensor(&hwSensor);
}

// ---------------------------------------------------------------------------
}; // namespace android

