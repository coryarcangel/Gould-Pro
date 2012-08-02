# Cory Arcangel
# 2007/8/9
#
# Small perl file to dump a Jpeg series from a score (filename comma samplenumberin comma samplenumberlength newline).
# Assumes our sample rate is 44100, also that there is a folder full of mp4s, and that this perl file sits outside that folder.
# Requires the program ffmpeg.
#
# "Doctor doctor, please, can't you see I just can't last" - UFO
#
#

#!/usr/bin/perl -w

use Class::Struct;
#use Term::ProgressBar 2.00;
use Cwd;

$filenum = 1; # Counter for naming files
$cur_samp = 0; # What sample (in frames) are we on?
$dir = getcwd; # What directory are we in?
$total_samples = 0; # Total samples we need to process


open(IFP,"$ARGV[0]") or die "could not open test.txt: $!"; # OPEN THE NOTE DATA FILE

$i = 0;
$j = 0;

# Input score file into arrays.

while (my $line = <IFP>)
{
($field1,$field2,$field3) = split ' ', $line;
$name[$i] = $field1;
$start[$i] =  $field2;
$sampsize[$i] = $field3;
$total_samples = $total_samples + $sampsize[$i];
$i++;
}

# Set up variables for loop.

$i = 0;
$name = $name[$i]; #name of file
$start = $start[$i]; #start position 
$sampsize = $sampsize[$i]; #no of samples

# Calculate the number of seconds we need to process
#my $progress = Term::ProgressBar->new((($total_samples / 44100) * 30));
$total_seconds = ($total_samples / 44100);

# make "MovieSeq" directory if it doesn't already exist
mkdir "MovieSeq", 0777 unless -d "MovieSeq";

# if it does exist, delete the files in there
system("rm MovieSeq/* &> /dev/null");

# Builds our video file and throw each individual JPEG at 30fps in a directory called MovieSeq......

for ($z = 0; $z < ($total_seconds * 30); $z++){
	
	copyfile($cur_samp, $name, $start);
	
	$cur_samp += 1470;
	
	if ($cur_samp > $sampsize){		
		
		$cur_samp = $cur_samp - $sampsize; # Reset cur_samp to the difference of it and the size of the note.
		$j++; # Increment the note we are on
		$name = $name[$j]; # Name of file
		$start = $start[$j]; # Start position 
		$sampsize = $sampsize[$j]; # No of samples
		&progress;
	}	
}

sub copyfile { #subroutine to copy file
	
	my ($cur_samp, $name, $start) = @_;
	
	#chop($name);
	
	$copyframe = (($start + $cur_samp) / 44100); # Convert samples 2 seconds
	
	# Place approiate Jpg into MOvieSeq folder
	
	if($filenum < 10){
		system("./ffmpeg -i mp4/" . $name . ".mp4 -ss " . $copyframe . " -t 1 -r 1 -y $dir/MovieSeq/foo_thumb_%03d.jpg &> /dev/null");
		system("mv $dir/MovieSeq/foo_thumb_001.jpg $dir/MovieSeq/mov000$filenum.jpg");
		
	}
	
	if($filenum >= 10 && $filenum < 100){
		system("./ffmpeg -i mp4/" . $name . ".mp4 -ss " . $copyframe . " -t 1 -r 1 -y $dir/MovieSeq/foo_thumb_%03d.jpg &> /dev/null");
		system("mv $dir/MovieSeq/foo_thumb_001.jpg $dir/MovieSeq/mov00$filenum.jpg");
		
	}
	
	if($filenum >= 100 && $filenum < 1000){
		system("./ffmpeg -i mp4/" . $name . ".mp4 -ss " . $copyframe . " -t 1 -r 1 -y $dir/MovieSeq/foo_thumb_%03d.jpg &> /dev/null");
		system("mv $dir/MovieSeq/foo_thumb_001.jpg $dir/MovieSeq/mov0$filenum.jpg");
		
	} 
	
	if($filenum >= 1000){
		system("./ffmpeg -i mp4/" . $name . ".mp4 -ss " . $copyframe . " -t 1 -r 1 -y $dir/MovieSeq/foo_thumb_%03d.jpg &> /dev/null");
		system("mv $dir/MovieSeq/foo_thumb_001.jpg $dir/MovieSeq/mov$filenum.jpg");
		
	}
		
	$filenum++;
	#$progress->update($_);
	

}

sub round { #subroutine for rounding numbers
    my($number) = shift;
    return int($number + .5);
}

sub progress
{
	print "\r";
	print int( ($z / ($total_seconds * 30)) * 100) . "% Complete";
	
}