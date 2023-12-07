#include "a4_util.h"

IOException::IOException() : std::exception(), message("Failed!"), errorState(8)
{}

IOException::IOException(const char* pMessage, std::ios_base::iostate rdstate) : std::exception(), message(pMessage), errorState(0)
{
	// Transform the error state into an integer that can be passed to the return value of main.
	if (std::ios_base::eofbit & rdstate)
		errorState |= 1;
	if (std::ios_base::failbit & rdstate)
		errorState |= 2;
	if (std::ios_base::badbit & rdstate)
		errorState |= 4;
  
  std::stringstream tmp;
  tmp << message << "\n" << "Exited with error code: " << errorState;
  what_ = tmp.str();
}


const char* IOException::what() const noexcept
{
  return what_.c_str();
}


unsigned IOException::getErrorState() const
{
	return errorState;
}


/*
* @var TranslationTableAminoAcids::CHAR_TO_INT
* @brief Mapping from residue to index.
* @signature const unsigned TranslationTableAminoAcids::CHAR_TO_INT[256];
* @note Defintion of static array must be outside of class. Use the functions toId and toAminoAcid respectively.
* @see toId();
* @see toAminoAcid();
*/
const unsigned TranslationTableAminoAcids::CHAR_TO_INT[256] =
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 8, 9, 10, 11, 0,
    // A, B, C, D, E, F, G, H, I, J, K, L,  M,  N, O
    12, 13, 14, 15, 16, 0, 17, 18, 0, 19, 0, 0, 0, 0, 0, 0,
    // P,  Q,  R,  S,  T, U,  V,  W, X,  Y, Z
    0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 8, 9, 10, 11, 0,
    //   a, b, c, d, e, f, g, h, i, j, k, l,  m, n, o
    12, 13, 14, 15, 16, 0, 17, 18, 0, 19, 0, 0, 0, 0, 0, 0,
    // p,  q,  r,  s,  t, u,  v,  w, x,  y, z
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


/*
* @var TranslationTableAminoAcids::INT_TO_CHAR
* @brief Mapping from index to residue.
* @signature const char TranslationTableAminoAcids::INT_TO_CHAR[TranslationTableAminoAcids::ALPHABET_SIZE];
* @note Defintion of static array must be outside of class. Use the functions toId and toAminoAcid respectively.
* @see toId();
* @see toAminoAcid();
*/
const char TranslationTableAminoAcids::INT_TO_CHAR[TranslationTableAminoAcids::ALPHABET_SIZE] =
{ 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y' };

//==================================================================
// Member functions.
//==================================================================


// Definition of the member function ScoreMatrix::load.
void ScoreMatrix::load(const char* fileName)
{
    std::ifstream matrixFile(fileName);  // Open ifstream for passed file.
    if (!matrixFile.good())  // Check stream could be opened.
        throw IOException("Could not read file!", matrixFile.rdstate());

    std::string buffer;  // buffer used to getline.
    std::vector<std::string> tokenized;  // tokenized string.

    std::vector<char> aminoAcids;  // Determines order of amino acids in read matrix.

    if (!getline(matrixFile, buffer).good())
        throw IOException("Failed to load the score matrix!", matrixFile.rdstate());
    splitString(tokenized, buffer, ' ');

    // Get first line consising of residue names.
    for (std::vector<std::string>::iterator it = tokenized.begin(); it != tokenized.end(); ++it)
        aminoAcids.push_back((*it).at(0));

    // Parse remaining file.
    while(getline(matrixFile, buffer).good())  // Reads line by line.
    {
        tokenized.clear();
        splitString(tokenized, buffer, ' ');
        // Fill the matrix for current row in the matrix.
        unsigned row = toId(tokenized[0][0]) * TranslationTableAminoAcids::ALPHABET_SIZE;
        for (unsigned i = 1; i < tokenized.size(); ++i)
            data[row + toId(aminoAcids[i - 1])] = atoi(tokenized[i].c_str());
    }
    
    if (!matrixFile.eof())
        throw IOException("Failed to load the score matrix!", matrixFile.rdstate());
}

// Definition of the member function ScoreMatrix::score.
int ScoreMatrix::score(char valA, char valB) const
{
    return data[(toId(valA) * TranslationTableAminoAcids::ALPHABET_SIZE) + toId(valB)];
}

// Print AminoAcid matrix.
void ScoreMatrix::print()
{
    std::cout << " ";
    for (unsigned j = 0; j < TranslationTableAminoAcids::ALPHABET_SIZE; ++j)
    {
        std::cout << "  " << toAminoAcid(j);
    }
    std::cout << std::endl;
    for (unsigned i = 0; i < TranslationTableAminoAcids::ALPHABET_SIZE; ++i)
    {
        std::cout << toAminoAcid(i);
        for (unsigned j = 0; j < TranslationTableAminoAcids::ALPHABET_SIZE; ++j)
        {
            int val = score(toAminoAcid(i), toAminoAcid(j));
            if (val < 0 || val > 10)
                std::cout << " ";
            else
                std::cout << "  ";
            std::cout << val;
        }
        std::cout << std::endl;
    }
}

//==================================================================
// Global functions.
//==================================================================

/*
 * @fn toInt
 * @brief Maps an amino acid one-to-one to an unsigned identifier in the range [0, ..., ALPHABET_SIZE).
 * @signature unsigned toInt(char value);
 *
 * @param value The amino acid to get the idendifier for.
 * @return The mapped position.
 * @see toAminoAcid
 */
unsigned toId(char val)
{
    return TranslationTableAminoAcids::CHAR_TO_INT[static_cast<unsigned>(val)];
}

/*
 * @fn toAminoAcid
 * @brief Maps an identifier to its corresponding amino acid.
 * @signature char toAminoAcid(unsigned value);
 *
 * @param value The identifier to query the amino acid for.
 * @return The mapped amino acid, or 'X' if the identifier is unknown.
 * @see toInt
 */
char toAminoAcid(unsigned index)
{
    if (index >= TranslationTableAminoAcids::ALPHABET_SIZE)
        return 'X';  // Return unknown amino acid.
    return TranslationTableAminoAcids::INT_TO_CHAR[index];
}

/*
 * @fn splitString
 * @brief Splits a given string by the given delimiter and stores the split strings in target.
 * @signature void splitString(vector<string> & target, string const & source, char delim);
 *
 * @param target The target vector containing the split string. Note this vector can be empty, if the source is empty. Will be modified!
 * @param source The source to be split.
 * @param delim The delimitter used to split the string. Defaults to ' '.
 */
void splitString(std::vector<std::string> & target, std::string const & source, char delim = ' ')
{
    typedef std::string::const_iterator TStringIter;

    std::string buffer;
    for (TStringIter it = source.begin(); it != source.end(); ++it)
    {
        if (*it == delim)
        {
            if (buffer.empty())
                continue;
            target.push_back(buffer);
            buffer = "";
            continue;
        }
        buffer += *it;
    }
    if (!buffer.empty())
        target.push_back(buffer);
}
