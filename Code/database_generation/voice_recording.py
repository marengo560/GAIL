# import required libraries
import sounddevice as sd
from scipy.io.wavfile import write
import wavio as wv
import os
from playsound import playsound
  

# Sampling frequency
freq = 16000
i=0
# Recording duration in seconds
duration = 1
word_name=input("Enter the word: ")
os.mkdir(word_name)
# to record audio from
# sound-device into a Numpy
#Word_list = ["GAIL","Track","Light","Stop","Solomon"]
while i<50:
    input("Enter key to record")
    recording = sd.rec(int(duration * freq),
                       samplerate=freq, channels=2)

    # Wait for the audio to complete
    sd.wait()
    text =word_name+str(i)+".wav"
    write(os.path.join(word_name,text), freq, recording)
    # for playing note.wav file
    #temp= input("enter r for replay or enter d to delete: ")
    #if temp=="r":
     #   os.system("afplay"+"{}/{}".format(word_name,text))
    #elif temp=="d":
     #   os.remove(os.path.join(word_name,text))
      #  i-=1

    i+=1
