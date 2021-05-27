from gtts import gTTS as g
import os
import playsound

os.chdir("..")

f=open('req.txt', 'r')
mytext=f.read();

language='en';

output=g(text=mytext,tld='com',lang=language,slow=False);

output.save("test.mp3");
print(mytext);
playsound.playsound("test.mp3");

os.system("del test.mp3");