#perl gould-available.pl allfiles.txt 46 5000

# Open the score file.
open(IFP,"$ARGV[0]") or die "Opening scorefile failed: $!"; 

while (my $line = <IFP>)
{
chomp($line);
	if ($line == "nofile" || $line == "nofiledoubling")
	{
	}
	else
	{
	open(IFP2,"transcriptions\/$line.txt") or die "Opening transcription file failed: $!"; 
		while (my $line2 = <IFP2>)
		{
		($note,$samplestart,$sampleend) = split ' ', $line2;
			if ($note == $ARGV[1] && ($sampleend - $samplestart) >= $ARGV[2])
			{
			print $line . " " . $samplestart . " " . $note . " " . ($sampleend - $samplestart) . "\n";
			}
		}
	}
}