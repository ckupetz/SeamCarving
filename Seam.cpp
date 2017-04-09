#include <iostream>
#include <vector>
#include <fstream>

/*
	Output 2d Matrix of Integers
*/
void OutputMatrix(std::vector<std::vector<int> > &matrix) {
	
	std::cout << "\n\nMATRIX\n";
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

/*
	Create new Matrix
*/
void FillFinalMatrix(std::vector<std::vector<int> > &ValueMatrix, std::vector<std::vector<int> > &FinalMatrix) {

	//Boolean the indicate whether the seam to be removed has been hit
  bool SeamFlag = false;

	//Iterate through the matrices and copy everything but the '-1's to the new Matrix
  for (int i = 0; i < ValueMatrix.size(); i++) {
    for (int j = 0; j < ValueMatrix[i].size(); j++) {
     if (ValueMatrix[i][j] == -1) SeamFlag = true;
     else { FinalMatrix[i][j-SeamFlag] = ValueMatrix [i][j]; }
   }
   SeamFlag = false;
 }
}

/*
  Read in Array from input .pgm file
*/
void FillValueMatrix(std::vector<std::vector<int> > &matrix, std::ifstream &input) {
  //Read in the pgm values by iterating through the empty matrix and reading in integers
  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix[i].size(); ++j) {
      int temp;
      input >> temp;
      matrix[i][j] = temp;
    }
  }
}

/*
	Create Energy Matrix & Cumulative Energy Matrix from Value Matrix
*/
void FillEnergyMatrices(std::vector<std::vector<int> > &ValueMatrix, std::vector<std::vector<int> > &EnergyMatrix, std::vector<std::vector<int> > &CumulativeMatrix) {
	
  //Iterate through the Matrix
  for (int i = 0; i < ValueMatrix.size(); ++i) {
    for (int j = 0; j < ValueMatrix[i].size(); ++j) {

     //Set dimensional integers to reference
     int Y_HEIGHT = ValueMatrix.size();
     int X_WIDTH = ValueMatrix[i].size();

     //Booleans to check whether the indices are on the edges of the matrix
     bool widthMin   = (j == 0);
     bool widthMax   = (j == X_WIDTH - 1);
     bool heightMin  = (i == 0);
     bool heightMax  = (i== Y_HEIGHT - 1);

       //Setting initial values for possible seam paths
     int leftUp      =  INT_MAX, 
     rightUp      =  INT_MAX,
     Up      =  0;

     //Starting Energy for matrix[i][j] is the initial entry in the Value Matrix
     int Energy      = ValueMatrix[i][j];

     //Initialize Final Energy value to 0
     int FinalEnergy = 0;

     //Adding Energy values depending on whether the indices are on the edges of the matrix
     if (!widthMax)  { FinalEnergy += abs(Energy - ValueMatrix[i][j+1]); }
     if (!widthMin)  { FinalEnergy += abs(Energy - ValueMatrix[i][j-1]); }
     if (!heightMax) { FinalEnergy += abs(Energy - ValueMatrix[i+1][j]); }
     if (!heightMin) { FinalEnergy += abs(Energy - ValueMatrix[i-1][j]); }

     //Setting Energy Matrix and Cumulative Energy Matrix values
     EnergyMatrix[i][j] = FinalEnergy;
     CumulativeMatrix[i][j] = FinalEnergy;

     //If we aren't on the top level (for Cumulative Energy Matrix)
     if (i > 0) {

     //If we aren't on the far left or right of the matrix, calculate the possible values
      if (!widthMin)  {  leftUp = FinalEnergy + CumulativeMatrix[i-1][j-1]; }
      if (!widthMax)  { rightUp = FinalEnergy + CumulativeMatrix[i-1][j+1]; }
      Up = FinalEnergy + CumulativeMatrix[i-1][j];

     //Set the Cumulative Matrix at indices [i][j] to the minimum
      CumulativeMatrix[i][j] = std::min({leftUp, rightUp, Up});
    }

  }
}
}

void FindSeam(std::vector<std::vector<int> > &CumulativeMatrix, std::vector<std::vector<int> > &ValueMatrix, bool Rotated) {

  //Set dimensional integers to reference
  int Y_HEIGHT = CumulativeMatrix.size();
  int X_WIDTH  = CumulativeMatrix[0].size();

  //Find minimum index in bottom row of matrix to start seam from
  int minimum_index = 0;

  //Iterate through the matrix's bottom row and find the minimum index
  for (int i = 1; i < X_WIDTH; ++i) {;
    if (Rotated) {
      if (CumulativeMatrix[Y_HEIGHT-1][minimum_index] >= CumulativeMatrix[Y_HEIGHT-1][i]) minimum_index = i;
    }
    else {
      if (CumulativeMatrix[Y_HEIGHT-1][minimum_index] > CumulativeMatrix[Y_HEIGHT-1][i]) minimum_index = i;
    }
  }

  //Set value
  ValueMatrix[Y_HEIGHT-1][minimum_index] = -1;

	//Starting from the bottom, take the minimum value between LEFTUP, UP, and RIGHTUP
  for (int i = Y_HEIGHT - 1; i >= 0; --i) {

   //Booleans for checking whether you're on the edge of the matrix
   bool widthMin  = (minimum_index == 0);
   bool widthMax  = (minimum_index == X_WIDTH - 1);

    //Integers
   int leftUp     =  INT_MAX, 
   rightUp     =  INT_MAX,
   Up     =  0;

   if (i > 0) {

     //If you aren't on the edges, initialize the leftUp/rightUp as their actual values
     if (!widthMin)  {  leftUp = CumulativeMatrix[i-1][minimum_index-1]; }
     if (!widthMax)  { rightUp = CumulativeMatrix[i-1][minimum_index+1]; }
     Up = CumulativeMatrix[i-1][minimum_index];
     
     //Changing priority in the case of a tie depending on whether you have the transposed matrix or not
     if (!Rotated) {
       if      (leftUp ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index-1]  = -1; --minimum_index; }
       else if (    Up ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index]    = -1;                  }
       else  																							  { ValueMatrix[i-1][minimum_index+1]  = -1; ++minimum_index; }
     }
     else {
       if      (rightUp ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index+1] = -1; ++minimum_index; }
       else if (    Up ==  std::min({leftUp, rightUp, Up}))  { ValueMatrix[i-1][minimum_index]   = -1;                  }
       else                                                  { ValueMatrix[i-1][minimum_index-1] = -1; --minimum_index; }          
     }
   } 
 }
}

/*
  Rotating a matrix 90 degrees clockwise
*/
void Rotate90(std::vector<std::vector<int> > &ValueMatrix)
{
	//Temporary Matrix for rotation
  std::vector<std::vector<int>> RotatedMatrix    (ValueMatrix[0].size(),  std::vector<int> (ValueMatrix.size(),   0  ));

  //Set height/width, iterate through and fill in new values
  int X_WIDTH = ValueMatrix[0].size();
  int Y_HEIGHT = ValueMatrix.size();
  for(int i=0; i<X_WIDTH; i++) {
   for(int j=0; j<Y_HEIGHT; j++) {
     RotatedMatrix[i][j] = ValueMatrix[Y_HEIGHT-1-j][i];
   }
 }
 
 //Clear current matrix and set it to new one
 ValueMatrix.clear();
 ValueMatrix = RotatedMatrix;
}

/*
  Rotating a matrix 90 degrees counterclockwise
*/
void RotateNeg90(std::vector<std::vector<int> > &ValueMatrix)
{
  //Temporary Matrix for rotation
  std::vector<std::vector<int>> RotatedMatrix    (ValueMatrix[0].size(),  std::vector<int> (ValueMatrix.size(),   0  ));

  //Set height/width, iterate through and fill in new values
  int X_WIDTH = ValueMatrix[0].size();
  int Y_HEIGHT = ValueMatrix.size();
  for(int i=0; i<X_WIDTH; i++) {
    for(int j=0; j<Y_HEIGHT; j++) {
      RotatedMatrix[i][j] = ValueMatrix[j][X_WIDTH - 1 - i];
    }
  }
  
  //Clear current matrix and set it to new one
  ValueMatrix.clear();
  ValueMatrix = RotatedMatrix;
}

/*
  Removes [SEAMS] number of seams from the 
*/
void RemoveSeam(int &Y_HEIGHT, int &X_WIDTH, int SEAMS, std::vector<std::vector<int> > &ValueMatrix, bool Rotated) {
  
  //Iterate through [SEAMS] number of times
  for (int i=0; i<SEAMS; ++i) {

    //Initialize vectors needed for Energy/Cumulative Energy/Final Matrices
    std::vector<std::vector<int>> EnergyMatrix     (Y_HEIGHT, std::vector<int> (X_WIDTH,    0  ));
    std::vector<std::vector<int>> CumulativeMatrix (Y_HEIGHT, std::vector<int> (X_WIDTH,    0  ));
    std::vector<std::vector<int>> FinalMatrix      (Y_HEIGHT, std::vector<int> (X_WIDTH-1,  0  ));

    //Fill the Energy Matrices
    FillEnergyMatrices (ValueMatrix, EnergyMatrix, CumulativeMatrix);

    //Find the Seam to be deleted
    FindSeam           (CumulativeMatrix, ValueMatrix, Rotated);

    //Fill in the new Matrix
    FillFinalMatrix(ValueMatrix, FinalMatrix);

    //Decrement the width for the new matrices
    --X_WIDTH;

    //Set the Value Matrix to the new Values/Dimensions
    ValueMatrix.clear();
    ValueMatrix = FinalMatrix;

  }
}

/*
  Output final matrix values to file with ofstream object
*/
void OutputFinalMatrix(const std::vector<std::vector<int>> &matrix, std::ofstream &output, int MAX_VALUE) {
  
  //Output the literal "P2"/Dimensions
  output << "P2\n";
  output << matrix[0].size() << " " << matrix.size() << "\n";
  output << MAX_VALUE << "\n";
  
  //Iterate through Matrix and output values to file
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      output << matrix[i][j] << " ";
    }
    if (i != matrix.size()-1) output << "\n";
  }
}

/*
  Get values from infile
*/
void GetValuesFromFile(int &X_WIDTH, int &Y_HEIGHT, int &MAX_VALUE, std::ifstream &input) {
  //Get the literal "P2" from the .pgm file
  std::string P2;
  std::getline(input, P2);

  //Skip the commented line in the .pgm file
  input.ignore(500, '\n');

  //Take in Dimensions from infile
  input >> X_WIDTH >> Y_HEIGHT;
  input >> MAX_VALUE;

}



