#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cstdlib>

//==================================================================
// Forwards
//==================================================================

unsigned toId(char);  
char toAminoAcid(unsigned);
void splitString(std::vector<std::string> &, std::string const &, char);

//==================================================================
// Classes, Tags, Enums
//==================================================================

/*
 * @class IOException
 * @inherits std::excetption
 * @brief Basic exception handling on failed IO operations.
 */
struct IOException : public std::exception
{

    /*
     * @fn IOException::IOException
     * @brief Constructor.
     * @signature IOException();
     */
    IOException();

    /*
     * @fn IOException::IOException
     * @brief Constructor.
     * @signature IOException(const char* mes, ios_base::iostate rdstate);
     * @param mes The message to be printed.
     * @param rdstate The io error state of the stream. (See fstream::rdstate())
     */
    IOException(const char* pMessage, std::ios_base::iostate rdstate);

    /*
     * @brief Overloads the exception::what() member function.
     * @return const char* The error message.
     */
    const char* what() const noexcept override;

    /*
     * @fn IOExeption::getErrorState
     * @brief Returns the error state for which this exception was thrown.
     * @signature unsigned getErrorState() const;
     * @return unsigned The error state.
     */
    unsigned getErrorState() const;

private:
    std::string message;  // The internal message stored.
    std::string what_;    // The internal what
    unsigned errorState;  // The internal error state stored.
};


/*
 * @class TranslationTableAminoAcids
 * @brief Translation table used to rapidly switch between residue name and position within matrix and vice versa.
 * @note Only implements static tables. The alphabet size can be accessed by TranslationTableAminoAcids::ALPHABET_SIZE.
 */
struct TranslationTableAminoAcids
{
    static const unsigned ALPHABET_SIZE = 20;  // The static alphabet size.

    static const unsigned CHAR_TO_INT[256];  // Mapping from residue to index.

    static const char INT_TO_CHAR[ALPHABET_SIZE];  // Mapping from index to residue.
};



/*
 * @class ScoreMatrix
 * @brief Scoring matrix used for protein alphabets.
 */
class ScoreMatrix
{
public:

    /*
     * @fn ScoreMatrix::ScoreMatrix
     * @brief Constructor.
     */
    ScoreMatrix()
    {};

    /*
     * @fn ScoreMatrix::load
     * @brief Loads the scoring matrix from disk.
     * @signature void load(const char* filename);
     * @param filename The name of the file to load the matrix from.
     * @throws IOException if file couldn't be opened or read.
     */
    void load(const char*);

    /*
     * @fn ScoreMatrix::score
     * @brief Returns the score for to residues.
     * @signature int score(char res1, char res2);
     * @param res1 A residue belonging to the alphabet.
     * @param res2 A residue belonging to the alphabet.
     * @return Returns the score between the two residues according to the scoring matrix.
     */
    int score(char, char) const;

    /*
     * @fn ScoreMatrix::print
     * @brief Prints the scoring matrix.
     * @signature void print();
     */
    void print();

    // Member variables:
private:
    int data[TranslationTableAminoAcids::ALPHABET_SIZE * TranslationTableAminoAcids::ALPHABET_SIZE];  // Data table to store actual data.
};

