Example yaml to use in esphome device config:

external_components:
  - source:
      type: git
      url: https://github.com/swoboda1337/gy39-esphome
      ref: main
    refresh: 1d

    sensor:
      - platform: gy39
        temperature:
          name: "Temperature"
        humidity:
          name: "Humidity"
        pressure:
          name: "Pressure"
        altitude:
          name: "Altitude"
        lux:
          name: "Lux"
        update_interval: 60s

    i2c:
      sda: 21
      scl: 22


