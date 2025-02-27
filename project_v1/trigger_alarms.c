#include <stdio.h>
#include <wiringPiI2C.h>
#include <unistd.h>

// DS1307 RTC I2C address
#define RTC_ADDR 0x68

// Register addresses for DS1307
#define SEC_REG 0x00
#define MIN_REG 0x01
#define HOUR_REG 0x02
#define DAY_REG 0x03
#define DATE_REG 0x04
#define MONTH_REG 0x05
#define YEAR_REG 0x06

// Function to convert BCD to decimal
int bcd_to_decimal(int bcd) {
    return (bcd / 16 * 10) + (bcd % 16);
}

int main() {
    int fd, sec, min, hour, day, date, month, year;

    // Initialize I2C communication with the DS1307 RTC
    fd = wiringPiI2CSetup(RTC_ADDR);
    if (fd == -1) {
        printf("Failed to initialize I2C communication.\n");
        return 1;
    }

    // Read the time registers from the DS1307
    sec = wiringPiI2CReadReg8(fd, SEC_REG);
    min = wiringPiI2CReadReg8(fd, MIN_REG);
    hour = wiringPiI2CReadReg8(fd, HOUR_REG);
    day = wiringPiI2CReadReg8(fd, DAY_REG);
    date = wiringPiI2CReadReg8(fd, DATE_REG);
    month = wiringPiI2CReadReg8(fd, MONTH_REG);
    year = wiringPiI2CReadReg8(fd, YEAR_REG);

    // Convert from BCD to decimal
    sec = bcd_to_decimal(sec);
    min = bcd_to_decimal(min);
    hour = bcd_to_decimal(hour);
    day = bcd_to_decimal(day);
    date = bcd_to_decimal(date);
    month = bcd_to_decimal(month);
    year = bcd_to_decimal(year);

    // Display the current time
    printf("Current Time: %02d:%02d:%02d\n", hour, min, sec);
    printf("Date: %02d/%02d/20%02d (Day: %d)\n", date, month, year, day);

    return 0;
}