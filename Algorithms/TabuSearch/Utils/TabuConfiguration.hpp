#pragma once

#include <cstdint>


class TabuConfiguration
{
    public:
    TabuConfiguration();
    ~TabuConfiguration() = default;

    void setTabuInterval(double intervalInSeconds);
    void enableDiversification();
    void disableDiversification();
    void setDiversificationCriteria(uint32_t numberOfIterations);
    void setTabuCadencyLength(uint32_t cadency);

    bool isDiversificationEnabled() const;
    double getTabuInterval() const;
    uint32_t getNumberOfIterations() const;
    uint32_t getCadencyLength() const;


    private:
    double tabuInterval;
    bool tabuDiversification;
    uint32_t tabuNumberOfIterations;
    uint32_t tabuCadencyLength;
};
