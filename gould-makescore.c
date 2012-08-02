// ./makescore score.txt allfiles.txt

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


int count_current_note_file = 0;
char * audio;
// ofstream outfile("outfile.aiff", ios::out | ios::binary);
int const allnotesvert = 1000;
int const notesvert = 1000;
int const myscorevert = 20000;
int const allfilesvert = 1000;

struct filename_noteon_noteoff
{
	string filename;
	int noteon;
	int noteoff;
};

struct numberavail_nno
{
	int numberavail;
	filename_noteon_noteoff notes[notesvert];
};

struct score
{
	int notepos;
	int note;
	string file;
	int length;
};

string allfiles[allfilesvert]; 
int allfiles_entries;
numberavail_nno allnotes[allnotesvert];  
int myscorein_entries;
score myscorein[myscorevert]; 
score myscorepossible[myscorevert];
score myscorefinal[myscorevert];
char* argv1; 
char* argv2; 
char* argv3;

void print_array(); 
void read_notedata_for_each_file(string current_file_to_read);  
void printallfiles();
void get_array_of_files(); 
void readscore();  
void print_myscorein(); 
void calculatescore();
void print_myscorefinal(); 
int getnote(int currentnote, int currentlength, string previousfile);
void generatescorefile();
void generatescoreaudio();

int main(int argc,char *argv[]) {
	
	argv1 = argv[1];
	argv2 = argv[2];
	argv3 = argv[3];
	get_array_of_files();  //this gets the files we have catalogued and stores in double struct allnotes
	//printallfiles(); // prints files we have to use
	//print_array(); //this prints the result of the above, all the notes have been organized
	readscore(); //this reads our score file into myscorein
	//print_myscorein(); //this prints our new score.
	 calculatescore();  //generate score.
	// print_myscorefinal();  //print out new score to terminal for debugging
	generatescorefile();	//print our score file to disk.  
	
	cout << argv1;
	return 0;
}


void generatescorefile(){

	ofstream myfile;
	myfile.open (argv3);
	
	for (int i = 0; i < myscorein_entries; i ++)
	{
		
		myfile << myscorefinal[i].file << " ";
		myfile << myscorefinal[i].notepos << " ";
		myfile << myscorefinal[i].length << "\n";
		
	}
	
	myfile.close();
}

void calculatescore()
{

	int currentnote;
	int currentlength;
	string previousfile = "null";
	int tempnote;
	
	// cout << "MYSCOREENTRIES--->" << myscorein_entries;
	
	for (int i = 0; i < myscorein_entries; i ++)
	{
	
		currentnote = myscorein[i].note;
		if (i == 0){
		currentlength = myscorein[i].length;
		}
		else{
		currentlength = myscorein[i].length - myscorein[i-1].length;
		}
		//currentlength = myscorein[i].length;
		tempnote = getnote(currentnote, currentlength, previousfile);
		
		if (tempnote == 1000){
			
			myscorefinal[i].notepos = 1000;	
			myscorefinal[i].file = "nofile";
			myscorefinal[i].note = currentnote;
			myscorefinal[i].length = currentlength;
			previousfile = "nofile";			
		
		}
		else if (tempnote == 1001){
			
			myscorefinal[i].notepos = 1000;	
			myscorefinal[i].file = "nofiledoubling";
			myscorefinal[i].note = currentnote;
			myscorefinal[i].length = currentlength;
			previousfile = "nofiledoubling";
			
		}
		else
		{
			myscorefinal[i].notepos = allnotes[currentnote].notes[tempnote].noteon;	
			myscorefinal[i].file = allnotes[currentnote].notes[tempnote].filename;
			myscorefinal[i].note = currentnote;
			myscorefinal[i].length = currentlength;
			previousfile = allnotes[currentnote].notes[tempnote].filename;	
		}
		
	
	}

}

int getnote(int currentnote, int currentlength, string previousfile){
	
	int notetoget;
	int currentnotel = currentnote;
	int currentlengthl = currentlength;
	string previousfilel = previousfile;
	int fits[100];
	int fitscounter = 0;	
	
	if (allnotes[currentnotel].numberavail == 0){return 1000;};
	
	for (int j = 0; j < allnotes[currentnotel].numberavail; j++)
	{
		
		int prolength = allnotes[currentnotel].notes[j].noteoff - allnotes[currentnotel].notes[j].noteon;
		
		if (currentlengthl < prolength && allnotes[currentnotel].notes[j].filename != previousfilel)
		{
		
			fits[fitscounter] = j;
			fitscounter = fitscounter + 1;
		
		}
		
	}

	if (fitscounter == 0){return 1001;}
	
	int randnote;
	randnote = rand() % (fitscounter + 1);			
	randnote = randnote - 1;
	if (randnote < 0){randnote = 0;}
	notetoget = fits[randnote];
		
	string ourinstrument;
	string availfilenames[fitscounter];	
	string lastfilename = "nousedyet";
	int percentfits = 0;  
	int fitshashcounter = 0;
	int fitshash[100]; 

	for (int i = 0; i < fitscounter; i++)
	{
		if (allnotes[currentnote].notes[fits[i]].filename != lastfilename &&  allnotes[currentnote].notes[fits[i]].filename != previousfile){
		availfilenames[percentfits] = allnotes[currentnote].notes[fits[i]].filename;
		lastfilename = allnotes[currentnote].notes[fits[i]].filename;
		percentfits = percentfits + 1;
		}
	}

	int randnote2;
	
	randnote2 = rand() % (percentfits + 1);			
	randnote2 = randnote2 - 1;	
	if (randnote2 < 0){randnote2 = 0;}	
	// cout << "\n RAND - " << rand() << " percentfits-" << percentfits << " randnote2-" << randnote2;  

	ourinstrument = availfilenames[randnote2];

	for (int j = 0; j < allnotes[currentnotel].numberavail; j++)
	{
		
		int prolength = allnotes[currentnotel].notes[j].noteoff - allnotes[currentnotel].notes[j].noteon;
		
		if (currentlengthl < prolength && allnotes[currentnotel].notes[j].filename != previousfilel && allnotes[currentnotel].notes[j].filename == ourinstrument)
		{

			fitshash[fitshashcounter] = j;
			fitshashcounter = fitshashcounter + 1;
			
		}
		
	}
		
	for (int i = 0; i < fitshashcounter; i++)
	{
		//cout << "\n" << allnotes[currentnote].notes[fitshash[i]].filename;
		//cout << " " << allnotes[currentnote].notes[fitshash[i]].noteon;
		//cout << "-" << fitshash[i];
	}
	
	int randnote3;
	randnote3 = rand() % (fitshashcounter + 1);			
	randnote3 = randnote3 - 1;
	if (randnote3 < 0){randnote3 = 0;}
	notetoget = fitshash[randnote3];
	//cout << "\n-rand-" << notetoget << "\n";
	
	return notetoget;	
	
}

void readscore()
{

	ifstream score (argv1, ios::in);
	
	if (score.is_open())
	{
			
		int tempcounter = 0;
				
		while(!score.eof())
		{
						
			score >> myscorein[tempcounter].note;  // get note we need
			score >> myscorein[tempcounter].length; // get the length
			tempcounter = tempcounter + 1;
		}
	
		myscorein_entries = tempcounter - 1;

	}
}

void print_myscorein(){
	
		cout << "\n*****************MYSCOREINFILE***************" << "\n";
		for (int i = 0; i < myscorein_entries; i++){
			 cout << myscorein[i].note << " " << myscorein[i].length << "\n";
		}
	
}

void print_myscorefinal(){

	cout << "\n*****************MYSCOREFINALFINALFINAL***************" << "\n";
	for (int i = 0; i < myscorein_entries; i++){
		cout << myscorefinal[i].note << " " << myscorefinal[i].length << " " << myscorefinal[i].file << " " << myscorefinal[i].notepos << "\n";
	}
	
}

void print_array(){
	
	for (int i = 0; i < 100; i++){
		
		if (allnotes[i].numberavail != 0){
			cout << "\nNumber Avail of Note " << i << " = " <<  allnotes[i].numberavail;
			cout << "\n++++++++++++++++++++\n";
		}
		
		for (int j = 0; j < allnotes[i].numberavail; j++){
			cout << allnotes[i].notes[j].noteon << " ";
			cout << allnotes[i].notes[j].noteoff << " ";
			cout << allnotes[i].notes[j].filename << " \n";
		}
	}
}	

void printallfiles()
{
	cout << "THESEFILESAREAVAILABLE\n";
	for (int i = 0; i <= allfiles_entries; i++)
	{
		cout << allfiles[i] << "\n";
	}		
}


void read_notedata_for_each_file(string current_file_to_read)
{
	
	string path = "transcriptions/";
	string current_file_to_read_disk = path + current_file_to_read + ".txt";
	
	ifstream current_note_file (current_file_to_read_disk.c_str(), ios::in);
	
	if (current_note_file.is_open())
	{
		int current_note_file_temp;
		int current_note_on_temp;
		int current_note_off_temp;
		
		while(!current_note_file.eof())
		{
		
			current_note_file >> current_note_file_temp;
			current_note_file >> current_note_on_temp;
			current_note_file >> current_note_off_temp;
			int notes_stored_temp;
			notes_stored_temp = allnotes[current_note_file_temp].numberavail;
			allnotes[current_note_file_temp].notes[notes_stored_temp].filename = current_file_to_read;
			allnotes[current_note_file_temp].notes[notes_stored_temp].noteon = current_note_on_temp;
			allnotes[current_note_file_temp].notes[notes_stored_temp].noteoff = current_note_off_temp;
			allnotes[current_note_file_temp].numberavail =  allnotes[current_note_file_temp].numberavail + 1;
			
		}
		
	}

	current_note_file.close();
	
}

void get_array_of_files()
{  
	
	ifstream allfiles_ifstream (argv2, ios::in);
	if (allfiles_ifstream.is_open())
	{
		
		int count = 0;
		while(!allfiles_ifstream.eof()){//
			
			allfiles_ifstream >> allfiles[count];
			read_notedata_for_each_file(allfiles[count]);
			count = count + 1;
			
			
		}
		
		allfiles_entries = count - 1;
		
	}
	allfiles_ifstream.close();
}



