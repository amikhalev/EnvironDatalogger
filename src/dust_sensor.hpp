#ifndef __DUST_SENSOR_HPP__
#define __DUST_SENSOR_HPP__

#include <stdint.h>

class DustSensor
{
  public:
    enum : uint8_t
    {
        DEFAULT_PIN = 2
    };

    enum : uint32_t
    {
        DEFAULT_SAMPLING_INTERVAL = (uint32_t)10 * 1000 * 1000 // 10 seconds
    };

    enum Error : int8_t
    {
        SUCCESS = 0,
        ERROR_INVALID_PIN = -1
    };

  private:
    uint8_t m_pin;
    uint32_t m_samplingInterval;

    volatile uint32_t mv_last_sample;
    volatile uint32_t mv_last_low;
    volatile uint32_t mv_low_accumulator;

    float m_low_ratio;
    float m_concentration;

    static void on_falling_edge();
    static void on_rising_edge();

    DustSensor();

  public:
    static DustSensor instance;

    inline void setSamplingInterval(uint32_t samplingInterval) { m_samplingInterval = samplingInterval; }
    inline uint32_t getSamplingInterval() { return m_samplingInterval; }

    /** begin with default pin */
    int8_t begin();

    /**
     * begin with the specified pin.
     * Must be on a pin which supports external interrupts, as they will be used.
     * @see attachInterrupt
     */
    int8_t begin(uint8_t pin);

    /**
     * Collects a sample right now
     */
    volatile void sample();

    /**
     * Collect a sample if the sampling interval has ellapsed
     */
    volatile void update();

    void end();

    inline const float getLowRatio() { return m_low_ratio; }
    inline const float getConcentration() { return m_concentration; }
};

extern DustSensor dust_sensor;

#endif // __DUST_SENSOR_HPP__