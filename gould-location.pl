#perl gould-location.pl paganini-master-score.csv 01:00:00

# Open the score file.
open(IFP,"$ARGV[0]") or die "Opening scorefile failed: $!"; 

$i = 1;
$samplenumbercounter = 0;
##($min,$sec,$secprecent) = split ':', $ARGV[1];
#$inSecs = ($min * 60) + ($sec + ($secprecent / 100));
#$samplenumberinput = int($inSecs * 44100);
$samplenumberinput = $ARGV[1];
$samplenumberlast = 0;

while (my $line = <IFP>)
{
($instrument,$samplestart,$length,$note) = split ' ', $line;
	
	if ($samplenumberinput >= $samplenumbercounter)
	{
	print $i . " " . $instrument . " " . $samplestart . " " . $length . " " . $note . " " . $samplenumbercounter . "\n";
	}
	else
	{
	print "line number of desired file is:" . ($i - 1) . "\n";
	last;
	}
	$samplenumbercounter = $samplenumbercounter + $length;
	$i = $i + 1;
}