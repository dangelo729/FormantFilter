#ifndef filter_formant_h_
#define filter_formant_h_

#include <Arduino.h>
#include <AudioStream.h>
class AudioFilterFormant : public AudioStream 
{
public:
    /**
     * @brief Enum representing different vowel sounds.
     */
    enum Vowel {
        VOWEL_AH,
        VOWEL_EE,
        VOWEL_I,
        VOWEL_OH,
        VOWEL_OO
        // Add other vowels later
    };

    /**
     * @brief Constructor for the FormantFilter class.
     * 
     * @details Initializes the FormantFilter object with one input channel and the inputQueueArray.
     */
    AudioFilterFormant(void) : AudioStream(1, inputQueueArray) {
        setVowel(VOWEL_AH);
        
    }

    /**
     * @brief Updates the FormantFilter object.
     */
    virtual void update(void);

    /**
     * @brief Sets the current vowel for the FormantFilter.
     * 
     * @param vowel The vowel to set.
     */
    void setVowel(Vowel vowel);

    /**
     * @brief Increments the bandpass filters towards a specific vowel for smooth transitions. Designed to be called in the loop.
     * 
     * @param vowel The vowel to increment towards.
     * @param speed The speed at which to increment the filters.
     */
    void incrementVowel(Vowel vowel, int speed);

private:
class BiquadBandpass {
public:
    BiquadBandpass() : frequency(1000.0f), q(1.0f), gain(1.0f) {
        updateCoefficients();
    }

    void setFrequency(float freq) {
        frequency = freq;
        updateCoefficients();
    }

    float getFrequency() const {
        return frequency;
    }

    void setQ(float qFactor) {
        q = qFactor;
        updateCoefficients();
    }

    float getQ() const {
        return q;
    }

    void setGain(float gainValue) {
        gain = gainValue;
    }

    float getGain() const {
        return gain;
    }

    void processAudio(audio_block_t *block) {
        if (!block) return;

        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            float in = block->data[i];
            float out = (in * a0 + z1) / b0;
            z1 = in * a1 + z2 - b1 * out;
            z2 = in * a2 - b2 * out;
            out *= gain;
            block->data[i] = static_cast<int16_t>(out);
        }
    }

private:
    void updateCoefficients() {
        float w0 = 2 * pi * frequency / AUDIO_SAMPLE_RATE_EXACT;
        float alpha = sin(w0) / (2 * q);
        float cosW0 = cos(w0);

        // Coefficients
        a0 = alpha;
        a1 = 0.0f;
        a2 = -alpha;
        b0 = 1.0f + alpha;
        b1 = -2.0f * cosW0;
        b2 = 1.0f - alpha;

        // Normalize coefficients
        a0 /= b0;
        a1 /= b0;
        a2 /= b0;
        b1 /= b0;
        b2 /= b0;

        // Reset state variables
        z1 = z2 = 0.0f;
    }
float pi = 3.14159265358979323846;
    float frequency;
    float q;
    float gain; // Added gain
    float a0, a1, a2, b0, b1, b2; // Added b0 for normalization
    float z1, z2;
};

    
    BiquadBandpass filter1, filter2, filter3;
    audio_block_t *inputQueueArray[1];
    // Formant frequencies for different vowels
    // From: https://www.classes.cs.uchicago.edu/archive/1999/spring/CS295/Computing_Resources/Csound/CsManual3.48b1.HTML/Appendices/table3.html
    float formant_frequencies[5][3] = {
        { 650.0f, 1080.0f, 2650.0f }, // AH
        { 400.0f, 1700.0f, 2600.0f }, // EE
        { 290.0f, 1870.0f, 2800.0f }, // I
        { 400.0f, 800.0f, 2600.0f }, // OH
        { 350.0f, 600.0f, 2700.0f } // OO
       
    };



float formant_qs[5][3]{
    { 14.0f * 1.3f, 12.0f * 1.3f, 9.0f * 1.3f }, // AH
    { 16.0f * 1.3f, 14.0f * 1.3f, 10.0f * 1.3f }, // EE
    { 25.0f * 1.3f, 12.0f * 1.3f, 10.0f * 1.3f }, // I
    { 25.0f * 1.3f, 14.0f * 1.3f, 10.0f * 1.3f }, // OH
    { 25.0f * 1.3f, 18.0f * 1.3f, 10.0f * 1.3f } // OO
};



/*
        float formant_qs[5][3]{
        { 14.0f, 12.0f, 9.0f }, // AH
        { 16.0f, 14.0f, 10.0f }, // EE
        { 25.0f, 12.0f, 10.0f }, // I
        { 25.0f, 14.0f, 10.0f }, // OH
        { 25.0f, 18.0f, 10.0f } // OO
      
    };
*/

/*
float formant_gains[5][3]{
    { 1.0f, 1.0f, 1.0f }, // AH
    { 1.0f, 1.0f, 1.0f }, // EE
    { 1.0f, 1.0f, 1.0f }, // I
    { 1.0f, 1.0f, 1.0f }, // OH
    { 1.0f, 1.0f, 1.0f } // OO
};
*/

    float formant_gains[5][3]{
        { 1.0f, 0.501f, 0.431f }, // AH
        { 1.0f, 0.21f, 0.29f }, // EE
        { 1.0f, .18f, .13f }, // I
        { 1.0f, .316f, 0.3f }, // OH
        { 1.0f, 0.1f, 0.14f } // OO
       
    };

};

#endif