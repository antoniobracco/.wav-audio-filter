# .wav audio filter

This programme allows the user to modify a .wav file using different filters such as:

amp : Amplificator.  
norm: Normalizer.  
eco : Gives echo effect according to delay and mix parameters.  
over: Gives overdrive effect.  
clip: Soft-clipping.  
chorus: Chorus effect according to frequency in hertz and deepness in seconds.   
flanger: Flanger filter.  
reverb:  Reverb filter.  

The programme was written in C.  

The user input works in the following way:

./Audio_filter input_filename output_filename filter params  

If there are no parameters, some will be placed by default.

i.e:  
Amplify sample1.wav by a factor of 2 and return file as output.wav  

./Audio_filter sample1.wav output.wav amp 2  

Parameters:

amp : float amplification.  
norm: float factor.  
eco : float delay, float mix.  
over: float distortion.  
clip: float clipping limit.  
chorus: float deepness, float frequency, float mix.   
flanger: float deepness, float intensity, float frequency, float mix.  
reverb: float deepness, float intensity, float mix.  
