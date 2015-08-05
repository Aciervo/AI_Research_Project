/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Solution:		Artificial_Intelligence_Project
//	Project:		Simple_Genetic_Algorithm_AI
//	Filename:		main.cpp
//	Author:			Andrew Ciervo
//	Created:		July 19, 2014
//	Modified:		July 26, 2014
//	Notes:			This project will be about an extremely simple genetic algorithm with the domain, 
//					or I like to think of it as the situation, of cards.  The goal would be to find
//					the gene, or pile, of cards that add up to a specific number, and the product of the
//					cards is close to another number. The numbers can be adjusted below as preprocessor 
//					macros which will be listed in the PREPROCESSOR MACROS section. Each pile of cards
//					holds 5 cards each and pile #1 correlates to the TARGET_SUM, while pile #2 correlates
//					to the TARGET_PRODUCT.
//
//	References:		http://www.slideshare.net/pbpimpale/genetic-algorithms-200688
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Section:		Preprocessor Macros
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	TARGET_SUM			30.0f	//36.0f
#define TARGET_PRODUCT		945.0f	//360.0f
#define	MAX_GENES			30
#define MAX_CARDS			10
#define MUTATION_RATE		30.0f	//0.1f		// Turned up these two rates to allow a solution to be found more often
#define RECOMBINATE_RATE	40.0f	//0.5f
#define TOURNEMENT_COUNT	1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Section:		Includes
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Section:		Global Variables
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool chromosomeFound = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Section:		Helper Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void printGene( unsigned int* gene )
{
	cout<< "Gene: ";
	for( unsigned int i = 0; i < MAX_CARDS; i++ )
		if( gene[i] == 0 )
			cout << i+1 << " ";

	for( unsigned int i = 0; i < MAX_CARDS; i++ )
		if( gene[i] == 1 )
			cout << i+1 << " ";
}

float calculateFitness( unsigned int* gene )
{
	int sum = 0, product = 1;
	float sum_error, product_error, combined_error;

	for( int i = 0; i < MAX_CARDS; i++ )
	{
		// Check to see which pile each number should go into
		if( gene[i] == 0)
			sum		+= (i+1);		// Calculate the sum
		else			
			product *= (i+1);		// Calculate the product
	}

	// Calculate the error for the sum and product
	sum_error		= (sum - TARGET_SUM) / TARGET_SUM;
	product_error	= (product - TARGET_PRODUCT) / TARGET_PRODUCT;
	combined_error	= abs(sum_error) + abs(product_error);

	return combined_error;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Function:		main
//	Return:			exitcode for success or failure
//	Params:			argc - commandline argument count
//					argv - commandline argument array of strings to parse for any reason needed
//	Notes:			Entry point to the program.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
	////////////////////////////////////////////////////////////////
	// Seed the random generator for unique genes
	srand(unsigned int(time(0)));			// Comment this and the next line to try and see unique results each time
	rand();								// But that doesn't guarentee it will find a chromosome...should print out 
											// the closest result based upon fitness

	////////////////////////////////////////////////////////////////
	// Declare array of genes and set up cards by sorting into piles
	unsigned int genes[MAX_GENES][MAX_CARDS] = {};
	for( unsigned int gene = 0; gene < MAX_GENES; gene++ )
	{
		for( unsigned int card = 0; card < MAX_CARDS; card++ )
		{
			if( rand() % 2 == 0 )			// Will put card in the first pile(SUMS)
				genes[gene][card] = 0;
			else							// Will put card in the second pile(PRODUCTS)
				genes[gene][card] = 1;
		}
	}

	////////////////////////////////////////////////////////////////
	// Step 1:		Selection( Tournement style )
	int winner, loser, fighter1, fighter2;
	for( int i = 0; i < TOURNEMENT_COUNT; i++ )
	{
		// Choose 2 fighters at random
		fighter1 = rand() % MAX_GENES + 1;
		fighter2 = rand() % MAX_GENES + 1;

		if( calculateFitness(genes[fighter1]) < calculateFitness(genes[fighter2]) )
		{
			winner = fighter1;
			loser = fighter2;
		}
		else
		{
			winner = fighter2;
			loser = fighter1;
		}

		for( int j = 0; j < MAX_CARDS; j++ )
		{
			////////////////////////////////////////////////////////////////
			// Step 2:		Recombination( Random chance for recombination )
			if( rand() < RECOMBINATE_RATE )
				genes[loser][j] = genes[winner][j];

			////////////////////////////////////////////////////////////////
			// Step 3:		Mutation (Random chance to mutate )
			if( rand() < MUTATION_RATE )
				genes[loser][j] = 1 - genes[loser][j];
		}

		////////////////////////////////////////////////////////////////
		// Step 4:		Check for 0.0% error rate( WE DID IT! )
		if( calculateFitness(genes[loser]) == 0.0f )
		{
			cout << "It took " << i << " to find a chromosome." << endl;
			printGene(genes[loser]);
			cout << endl;	
			chromosomeFound = true;
		}
	}

	////////////////////////////////////////////////////////////////
	// What was our closest gene
	if( chromosomeFound == false )
	{
		cout << "Sadly after " << TOURNEMENT_COUNT << " rounds a chromosome was not found.\nBut the closest gene we got was: " << endl;
		float gene[MAX_GENES] = {};
		int bestChance = 999999999;
		for( int i=0; i < MAX_GENES; i++ )
		{
			gene[i] = calculateFitness(genes[i]);

			if( bestChance > gene[i] )
				bestChance = i;
		}

		printGene(genes[bestChance]);
		cout << endl << endl;
	}

	////////////////////////////////////////////////////////////////
	cout << "Press any key to continue . . .";
	getchar();
	return 0;
}