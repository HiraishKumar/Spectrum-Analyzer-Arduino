#include <arduinoFFT.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "config.h"

double real[SAMPLES];
double imag[SAMPLES];
char data_avgs[xres];

int y_val;
int column, disp_value;
int peaks[xres];

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                 // Display object
arduinoFFT FFT = arduinoFFT();                                                  // FFT object

void setup() {
    ADCSRA = 0b11100101;                                                       // Set ADC to free running mode and set pre-scalar to 32 (0xe5)
    ADMUX = 0b00000000;                                                        // Use pin A0 and external voltage reference
    mx.begin();                                                                // Initialize display
    delay(50);                                                                 // Wait to get reference voltage stabilized
}

void loop() {
    //Sampling
    for (int i = 0; i < SAMPLES; i++) {
        while (!(ADCSRA & 0x10));                                               // Wait for ADC to complete current conversion, i.e., ADIF bit set
        ADCSRA = 0b11110101;                                                    // Clear ADIF bit so that ADC can do next operation (0xf5)
        int value = ADC - 512;                                                  // Read from ADC and subtract DC offset caused value
        real[i] = value / 8;                                                    // Copy to array after downscaling
        imag[i] = 0;
    }

    //FFT
    FFT.Windowing(real, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(real, imag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(real, imag, SAMPLES);

    // column Matching, Calculating the average between multiple column when step != 1         
    int step = (SAMPLES/2)/xres; 
    int c = 0;
    for (int i = 0; i < (SAMPLES / 2); i += step) {
        data_avgs[c] = 0;
        for (int k = 0; k < step; k++) {
            data_avgs[c] += real[i + k];
        }
        data_avgs[c] /= step;
        c++;
    }

    //Send to display according to measured value
    for (int i = 0; i < xres; i++) {
        data_avgs[i] = constrain(data_avgs[i], 0, 80);                          // Set max & min values for buckets
        data_avgs[i] = map(data_avgs[i], 0, 80, 0, yres);                       // Remap averaged values to yres
        y_val = data_avgs[i];

        peaks[i] = peaks[i] - DECAY;                                            // Decay by DECAY cosnt lights (default 1)
        if (y_val > peaks[i])
            peaks[i] = y_val;
        y_val = peaks[i];
        disp_value = ARR[y_val];
        column = 31 - i;
        mx.setColumn(column, disp_value);                                       // For left to right
    }
}
