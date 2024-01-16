

#include <Arduino.h>
#include "filter_formant.h"




void AudioFilterFormant::update(void) {
    audio_block_t *block, *tempBlock1, *tempBlock2, *tempBlock3;
    block = receiveWritable();
    if (!block) return;

    // Create temporary blocks for individual filter outputs
    tempBlock1 = allocate();
    tempBlock2 = allocate();
    tempBlock3 = allocate();
    if (!tempBlock1 || !tempBlock2 || !tempBlock3) {
        // Handle memory allocation failure
        if (tempBlock1) release(tempBlock1);
        if (tempBlock2) release(tempBlock2);
        if (tempBlock3) release(tempBlock3);
        release(block);
        return;
    }

    // Copy input block data to temporary blocks
    memcpy(tempBlock1->data, block->data, sizeof(block->data));
    memcpy(tempBlock2->data, block->data, sizeof(block->data));
    memcpy(tempBlock3->data, block->data, sizeof(block->data));

    // Process each temporary block with a different bandpass filter
    filter1.processAudio(tempBlock1);
    filter2.processAudio(tempBlock2);
    filter3.processAudio(tempBlock3);

    // Sum the filtered signals back into the original block
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        int32_t sum = tempBlock1->data[i] + tempBlock2->data[i] + tempBlock3->data[i];
        
        // Clipping prevention
        if (sum > 32767) sum = 32767;
        else if (sum < -32768) sum = -32768;
        
        block->data[i] = sum;
    }

    // Release the temporary blocks
    release(tempBlock1);
    release(tempBlock2);
    release(tempBlock3);

    // Transmit the processed block
    transmit(block);
    release(block);
}

//commented out to see if connecting filters in series works better than parallel
/*
void AudioFilterFormant::update(void) {
    audio_block_t *block, *tempBlock;
    block = receiveWritable();
    if (!block) return;  // No input block, exit

    // Create a temporary block for intermediate filter outputs
    tempBlock = allocate();
    if (!tempBlock) {
        release(block);
        return;  // Memory allocation failure, exit
    }

    // Process the input block with the first bandpass filter
    filter1.processAudio(block);

    // Copy the output of filter1 to tempBlock
    memcpy(tempBlock->data, block->data, sizeof(block->data));

    // Process tempBlock with the second bandpass filter
    filter2.processAudio(tempBlock);

    // Now, use tempBlock's output as input to filter3
    memcpy(block->data, tempBlock->data, sizeof(tempBlock->data));
    filter3.processAudio(block);

    // Release the temporary block
    release(tempBlock);

    // Transmit the processed block
    transmit(block);
    release(block);
}
*/
void AudioFilterFormant::setVowel(Vowel vowel)
{

    switch (vowel){
        case VOWEL_AH:
            // Set the filter coefficients for the given vowel
            filter1.setFrequency(formant_frequencies[0][0]);
            filter2.setFrequency(formant_frequencies[0][1]);
            filter3.setFrequency(formant_frequencies[0][2]);
            filter1.setQ(formant_qs[0][0]);
            filter2.setQ(formant_qs[0][1]);
            filter3.setQ(formant_qs[0][2]);
            filter1.setGain(formant_gains[0][0]);
            filter2.setGain(formant_gains[0][1]);
            filter3.setGain(formant_gains[0][2]);
            break;
        case VOWEL_EE:
            // Set the filter coefficients for the given vowel
            filter1.setFrequency(formant_frequencies[1][0]);
            filter2.setFrequency(formant_frequencies[1][1]);
            filter3.setFrequency(formant_frequencies[1][2]);
            filter1.setQ(formant_qs[1][0]);
            filter2.setQ(formant_qs[1][1]);
            filter3.setQ(formant_qs[1][2]);
            filter1.setGain(formant_gains[1][0]);
            filter2.setGain(formant_gains[1][1]);
            filter3.setGain(formant_gains[1][2]);
            break;
        case VOWEL_I:
            // Set the filter coefficients for the given vowel
            filter1.setFrequency(formant_frequencies[2][0]);
            filter2.setFrequency(formant_frequencies[2][1]);
            filter3.setFrequency(formant_frequencies[2][2]);
            filter1.setQ(formant_qs[2][0]);
            filter2.setQ(formant_qs[2][1]);
            filter3.setQ(formant_qs[2][2]);
            filter1.setGain(formant_gains[2][0]);
            filter2.setGain(formant_gains[2][1]);
            filter3.setGain(formant_gains[2][2]);
            break;
        case VOWEL_OH:
            // Set the filter coefficients for the given vowel
            filter1.setFrequency(formant_frequencies[3][0]);
            filter2.setFrequency(formant_frequencies[3][1]);
            filter3.setFrequency(formant_frequencies[3][2]);
            filter1.setQ(formant_qs[3][0]);
            filter2.setQ(formant_qs[3][1]);
            filter3.setQ(formant_qs[3][2]);
            filter1.setGain(formant_gains[3][0]);
            filter2.setGain(formant_gains[3][1]);
            filter3.setGain(formant_gains[3][2]);
            break;
        case VOWEL_OO:
            // Set the filter coefficients for the given vowel
            filter1.setFrequency(formant_frequencies[4][0]);
            filter2.setFrequency(formant_frequencies[4][1]);
            filter3.setFrequency(formant_frequencies[4][2]);
            filter1.setQ(formant_qs[4][0]);
            filter2.setQ(formant_qs[4][1]);
            filter3.setQ(formant_qs[4][2]);
            filter1.setGain(formant_gains[4][0]);
            filter2.setGain(formant_gains[4][1]);
            filter3.setGain(formant_gains[4][2]);
            break;
        default:
        break;
    // Set the filter coefficients for the given vowel
    }
}


void AudioFilterFormant::incrementVowel(Vowel vowel, int speed)
{ 
    switch (vowel) {
        case VOWEL_AH:
            if (filter1.getFrequency() < formant_frequencies[0][0]) {
                filter1.setFrequency(filter1.getFrequency() + speed);
                if(filter1.getFrequency() > formant_frequencies[0][0]){
                    filter1.setFrequency(formant_frequencies[0][0]);
                }
            } else if (filter1.getFrequency() > formant_frequencies[0][0]) {
                filter1.setFrequency(filter1.getFrequency() - speed);
                if(filter1.getFrequency() < formant_frequencies[0][0]){
                    filter1.setFrequency(formant_frequencies[0][0]);
                }
            } 

            
            if (filter2.getFrequency() < formant_frequencies[0][1]) {
                filter2.setFrequency(filter2.getFrequency() + speed);
                if(filter2.getFrequency() > formant_frequencies[0][1]){
                    filter2.setFrequency(formant_frequencies[0][1]);
                }
            } else if (filter2.getFrequency() > formant_frequencies[0][1]) {
                filter2.setFrequency(filter2.getFrequency() - speed);
                if (filter2.getFrequency() < formant_frequencies[0][1]) {
                    filter2.setFrequency(formant_frequencies[0][1]);
                }
            }
            
            if (filter3.getFrequency() < formant_frequencies[0][2]) {
                filter3.setFrequency(filter3.getFrequency() + speed);
                if (filter3.getFrequency() > formant_frequencies[0][2]) {
                    filter3.setFrequency(formant_frequencies[0][2]);
                }
            } else if (filter3.getFrequency() > formant_frequencies[0][2]) {
                filter3.setFrequency(filter3.getFrequency() - speed);
                if (filter3.getFrequency() < formant_frequencies[0][2]) {
                    filter3.setFrequency(formant_frequencies[0][2]);
                }
            }
            break;
            
        case VOWEL_EE:
            if (filter1.getFrequency() < formant_frequencies[1][0]) {
                filter1.setFrequency(filter1.getFrequency() + speed);
                if (filter1.getFrequency() > formant_frequencies[1][0]) {
                    filter1.setFrequency(formant_frequencies[1][0]);
                }
            } else if (filter1.getFrequency() > formant_frequencies[1][0]) {
                filter1.setFrequency(filter1.getFrequency() - speed);
                if (filter1.getFrequency() < formant_frequencies[1][0]) {
                    filter1.setFrequency(formant_frequencies[1][0]);
                }
            }
            
            if (filter2.getFrequency() < formant_frequencies[1][1]) {
                filter2.setFrequency(filter2.getFrequency() + speed);
                if (filter2.getFrequency() > formant_frequencies[1][1]) {
                    filter2.setFrequency(formant_frequencies[1][1]);
                }
            } else if (filter2.getFrequency() > formant_frequencies[1][1]) {
                filter2.setFrequency(filter2.getFrequency() - speed);
                if (filter2.getFrequency() < formant_frequencies[1][1]) {
                    filter2.setFrequency(formant_frequencies[1][1]);
                }
            }
            
            if (filter3.getFrequency() < formant_frequencies[1][2]) {
                filter3.setFrequency(filter3.getFrequency() + speed);
                if (filter3.getFrequency() > formant_frequencies[1][2]) {
                    filter3.setFrequency(formant_frequencies[1][2]);
                }
            } else if (filter3.getFrequency() > formant_frequencies[1][2]) {
                filter3.setFrequency(filter3.getFrequency() - speed);
                if (filter3.getFrequency() < formant_frequencies[1][2]) {
                    filter3.setFrequency(formant_frequencies[1][2]);
                }
            }
            break;
            
        case VOWEL_OH:
            if (filter1.getFrequency() < formant_frequencies[2][0]) {
                filter1.setFrequency(filter1.getFrequency() + speed);
                if (filter1.getFrequency() > formant_frequencies[2][0]) {
                    filter1.setFrequency(formant_frequencies[2][0]);
                }
            } else if (filter1.getFrequency() > formant_frequencies[2][0]) {
                filter1.setFrequency(filter1.getFrequency() - speed);
                if (filter1.getFrequency() < formant_frequencies[2][0]) {
                    filter1.setFrequency(formant_frequencies[2][0]);
                }
            }
            
            if (filter2.getFrequency() < formant_frequencies[2][1]) {
                filter2.setFrequency(filter2.getFrequency() + speed);
                if (filter2.getFrequency() > formant_frequencies[2][1]) {
                    filter2.setFrequency(formant_frequencies[2][1]);
                }
            } else if (filter2.getFrequency() > formant_frequencies[2][1]) {
                filter2.setFrequency(filter2.getFrequency() - speed);
                if (filter2.getFrequency() < formant_frequencies[2][1]) {
                    filter2.setFrequency(formant_frequencies[2][1]);
                }
            }
            
            if (filter3.getFrequency() < formant_frequencies[2][2]) {
                filter3.setFrequency(filter3.getFrequency() + speed);
                if (filter3.getFrequency() > formant_frequencies[2][2]) {
                    filter3.setFrequency(formant_frequencies[2][2]);
                }
            } else if (filter3.getFrequency() > formant_frequencies[2][2]) {
                filter3.setFrequency(filter3.getFrequency() - speed);
                if (filter3.getFrequency() < formant_frequencies[2][2]) {
                    filter3.setFrequency(formant_frequencies[2][2]);
                }
            }
            break;
            
        case VOWEL_OO:
            if (filter1.getFrequency() < formant_frequencies[3][0]) {
                filter1.setFrequency(filter1.getFrequency() + speed);
                if (filter1.getFrequency() > formant_frequencies[3][0]) {
                    filter1.setFrequency(formant_frequencies[3][0]);
                }
            } else if (filter1.getFrequency() > formant_frequencies[3][0]) {
                filter1.setFrequency(filter1.getFrequency() - speed);
                if (filter1.getFrequency() < formant_frequencies[3][0]) {
                    filter1.setFrequency(formant_frequencies[3][0]);
                }
            }
            
            if (filter2.getFrequency() < formant_frequencies[3][1]) {
                filter2.setFrequency(filter2.getFrequency() + speed);
                if (filter2.getFrequency() > formant_frequencies[3][1]) {
                    filter2.setFrequency(formant_frequencies[3][1]);
                }
            } else if (filter2.getFrequency() > formant_frequencies[3][1]) {
                filter2.setFrequency(filter2.getFrequency() - speed);
                if (filter2.getFrequency() < formant_frequencies[3][1]) {
                    filter2.setFrequency(formant_frequencies[3][1]);
                }
            }
            
            if (filter3.getFrequency() < formant_frequencies[3][2]) {
                filter3.setFrequency(filter3.getFrequency() + speed);
                if (filter3.getFrequency() > formant_frequencies[3][2]) {
                    filter3.setFrequency(formant_frequencies[3][2]);
                }
            } else if (filter3.getFrequency() > formant_frequencies[3][2]) {
                filter3.setFrequency(filter3.getFrequency() - speed);
                if (filter3.getFrequency() < formant_frequencies[3][2]) {
                    filter3.setFrequency(formant_frequencies[3][2]);
                }
            }
            break;
            
        default:
            break;
    }

}