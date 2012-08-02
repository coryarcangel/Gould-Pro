# Cory Arcangel
# 2007/8/9
#
# Small perl file to construct a aif file from a score (filename comma samplenumberin comma samplenumberlength newline).
# Assumes a folder full of aifS at 44100 samples / second, and that this perl file sit outside that folder.
# Requires the program sox.
#
# "Doctor doctor, please, can't you see I just can't last" - UFO
#
#

#!/usr/bin/perl -w

use Class::Struct;
use Data::Dumper;

# Open the score file.
open(IFP,"$ARGV[0]") or die "Opening scorefile failed: $!"; 

$total_number_of_files_to_process = 0;

$i = 0;
while (my $line = <IFP>)
{
	($field1,$field2,$field3) = split ' ', $line;
	$name[$i] = $field1;
	$start[$i] =  $field2;
	$sampsize[$i] = $field3;
	$i++;
}

# How many files do we need to process and adjust progress bar.

# Count through the files. Trim the segemnt we are adding. 
# Make sure it is 220 samples bigger on either side (soxs cross fade length at 44100)
# UNless we are on the first file, splice it onto the end of the previous finished file.

$count = $i;
for ($i = 0; $i < $count; $i++)
{

	if ($i == 0)
	{
		$sox_string = "\./sox aif/" . $name[$i] . ".aif trim1.aif trim " . $start[$i] . "s " . ($sampsize[$i] + 220) . "s" . "&> /dev/null";
		system("$sox_string");
		system("cp trim1.aif final.aif");
		$endsample_score = $sampsize[$i];
		&progress;
	}
	else
	{
		$sox_string = "\./sox aif/" . $name[$i] . ".aif add.aif trim " . ($start[$i] - 220) . "s " . ($sampsize[$i] + 440) . "s" . "&> /dev/null";
		$currentsamplesizemilli = ($endsample_score + 220) / 44100;
		system("$sox_string");
		$sox_splice = "\./sox final.aif add.aif final_add.aif splice " . $currentsamplesizemilli . ",0.005,0" . "&> /dev/null";
		system("$sox_splice");
		system("cp final_add.aif final.aif");
		$endsample_score = $sampsize[$i] + $endsample_score;
		&progress;
	}
}

# Clean up temp files, and remake our output.

system("rm final_add.aif");
system("rm add.aif");
system("rm trim1.aif");
system("mv final.aif $ARGV[1]");

sub progress
{
	print "\r";
	print int(($i / $count) * 100) . "% Complete";
}