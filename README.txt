********************************************************

Gould Pro V.02

Cory Arcangel - 2007-8-9-10-11

http://www.coryarcangel.com
	
Requirements:  ffmpeg, sox, g++, MAX/MSP runtime with fiddle object. 

********************************************************

gould-makeaudio.pl
gould-makevideo.pl
gould-makescore.c
gould-maketranscriptions.mxb64

These 4 scripts allow one to re-construct in video a musical score from sinipts of notes from other videos. They require an input of youtube videos at 340 * 240 resolution, and can reedit them together using sample based precision at 44100 samples / second. I should note here that there are probably 1,000,000 better ways to do this (Echo nest, Max/Jitter, Open Frameworks, etc, etc). So I would suggest searching a bit online to see if anything else is out there before using these bootleg very slow & clumsy non-realtime(!) scripts. Ps - All files here are compiled for Mac OSX 10.5.8 although all source is included for recompiling. 

Required:
ffmpeg
sox
g++
MAX/MSP runtime with fiddle object. 

How to:
1. Download your input vidoes.
2. Convert these into 320by240 mp4s and place then in an mp4 folder.
3. Extract aifs from the above mp4s and place in a aif folder.
4. Transcribe each video, by loading the aif into the gould-maketranscriptions.mxb64 which can be run using the free MAX/MSP runtime tool (requires fiddle object). 
	a. load sound file ("replace" for wav or aif, "import" for mp3). 
	b. let file play in its entirety
	c. click "format" button. 
	d. type keypad "1" to preview guessed aif note.
	e. type keypad "2" to hear midi note this corresponds to.
	f. if they are the same note, use sliders to adjust beginning and ending (if need be) and then type keypad "3" to OK this to transcription file. .
	g. if they are not the same note, skip. 
	h. once all notes have been Ok'd, click "write", save as name of your file with a .txt extension. 
	i. delete all entries that have no 4th note ending sample number. 
	j. reformat transcription file to be just midi number, space, sample number start, sample number end. 
5. Add each new transcription file name to our allfiles.txt with no extension. 
6. Prepare our score file. File format: note where middle C is 50, space, sample number at 44100 sample per second where the note starts at, new line. Sace as music-score.txt
7. Run ./gould-makescore music-score.txt allfiles.txt video-score.txt This generates video-score.txt which is a video score file of our youtube filehash, space, sample number at 44100 samples per second where you would like the in point of that video to be, sample number at 44100 samples per second of the length of that clip. Note: All files need 220 samples on the beginning and end for cross fading. Therefore, we can not start an in point at sample 0, and if we wanted a clip to last for a second it would have to be 44540 samples long (44100 + 220 + 220). Eventually I need to write a check for this condition. 
8. An example score has been provided named video-score.txt
g1 34750 5000
303 85201 1000
g1 346934 4000
Video g1 starts at sample 34750 and goes for 5000 samples, then video 303 starts at sample 85201 and goes for 1000, and then video g1 starts at 346934 and goes for 4000 samples. Remember to make sure that if your score references a file, the corresponding aif is in the aif folder and the corresponding mp4 is in the mp4 folder. So for our above score our directory should look like this:
gould-maketranscriptions.mxb64
gould-makevideo.pl
gould-makeaudio.pl
gould-makescore.c
MovieSeq/
aif/
	303.aif
	g1.aif
mp4/
	303.mp4
	g1.mp4
transcriptions/
	303.txt
	g1.txt
allfiles.txt
music-score.txt
video-score.txt
README.txt ps - You will find some extra files in the mp4 and aif folders, these are place holders if you don't have the notes the music-score calls for. 
7. To generate the audio file final.aif from video-score.txt and save audio file as final.aif: perl gould-makeaudio.pl video-score.txt final.aif
8. To generate a Jpeg sequence in folder MovieSeq from score.txt: perl gould-makevideo.pl video-score.txt
9. At the point you can glue the final.aif and Jpeg sequence together. I use Quicktime player, open sequence, and then open aif and "add to movie". 

