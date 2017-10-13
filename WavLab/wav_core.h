#ifndef WAV_CORE_H
#define WAV_CORE_H






// TODO: Implement all this in the form of a class.
// TODO: Use an exception system to control errors.
// TODO: Make support for 8-bit PSM, not just 16-bit.
// TODO: Write a function to change the tone of the voice. (Interestingly, it's not too difficult?)


// *********************************************************************
// * Error handling
// *********************************************************************

// Possible errors



// ************************************************************************
// * Functions for working with WAV file
// ************************************************************************

// Reads file 'filename' and puts header's data to 'header_ptr' address.
// Also checks header validity, returns 'WAV_OK' on success.


// Prints header's data from 'header_ptr' address.


// Reads file 'filename' and puts PCM data (raw sound data) to 'channels_data'.
// Also checks header validity, returns 'WAV_OK' on success.



#endif // WAV_CORE_H
