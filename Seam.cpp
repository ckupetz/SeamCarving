#include <iostream>
#include <vector>
#include <fstream>


#define RESET "\033[0m"
#define RED "\033[35m" 

/*
	Output 2d Matrix of Integers
*/
void OutputMatrix(std::vector<std::vector<int> > &matrix) {
	
	std::cout << "\n\nMATRIX\n";
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			if (matrix[i][j] == -1) std::cout << matrix[i][j] << " ";
			else                    std::cout << RED << matrix[i][j] << RESET << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

/*
	Create new Matrix
*/
void FillFinalMatrix(std::vector<std::vector<int> > &ValueMatrix, std::vector<std::vector<int> > &FinalMatrix) {
		
	bool SeamFlag = false;

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
	for (int i = 0; i < ValueMatrix.size(); ++i) {
    for (int j = 0; j < ValueMatrix[i].size(); ++j) {
       
       int Y_HEIGHT = ValueMatrix.size();
       int X_WIDTH = ValueMatrix[i].size();

       bool widthMin  = (j == 0);
       bool widthMax  = (j == X_WIDTH - 1);
       bool heightMin = (i == 0);
       bool heightMax = (i== Y_HEIGHT - 1);

       int leftUp     =  INT_MAX, 
          rightUp     =  INT_MAX,
               Up     =  0;

       int Energy     = ValueMatrix[i][j];

       int FinalEnergy = 0;

       if (!widthMax)  { FinalEnergy += abs(Energy - ValueMatrix[i][j+1]); }
       if (!widthMin)  { FinalEnergy += abs(Energy - ValueMatrix[i][j-1]); }
       if (!heightMax) { FinalEnergy += abs(Energy - ValueMatrix[i+1][j]); }
       if (!heightMin) { FinalEnergy += abs(Energy - ValueMatrix[i-1][j]); }

       EnergyMatrix[i][j] = FinalEnergy;
       CumulativeMatrix[i][j] = FinalEnergy;

       if (i > 0) {
        if (!widthMin)  {  leftUp = FinalEnergy + CumulativeMatrix[i-1][j-1]; }
        if (!widthMax)  { rightUp = FinalEnergy + CumulativeMatrix[i-1][j+1]; }
                              Up = FinalEnergy + CumulativeMatrix[i-1][j];

        CumulativeMatrix[i][j] = std::min({leftUp, rightUp, Up});
      }

    }
  }
}

void FindSeam(std::vector<std::vector<int> > &CumulativeMatrix, std::vector<std::vector<int> > &ValueMatrix, bool Rotated) {
  
  int Y_HEIGHT = CumulativeMatrix.size();
  int X_WIDTH  = CumulativeMatrix[0].size();

  /*
    Find minimum index in bottom row of matrix to start seam from
  */
  int minimum_index = 0;

  for (int i = 1; i < X_WIDTH; ++i) {;
    if (Rotated) {
      if (CumulativeMatrix[Y_HEIGHT-1][minimum_index] >= CumulativeMatrix[Y_HEIGHT-1][i]) minimum_index = i;
    }
    else {
      if (CumulativeMatrix[Y_HEIGHT-1][minimum_index] > CumulativeMatrix[Y_HEIGHT-1][i]) minimum_index = i;
    }
  }

  ValueMatrix[Y_HEIGHT-1][minimum_index] = -1;

	for (int i = Y_HEIGHT - 1; i >= 0; --i) {

       bool widthMin  = (minimum_index == 0);
       bool widthMax  = (minimum_index == X_WIDTH - 1);

       int leftUp     =  INT_MAX, 
          rightUp     =  INT_MAX,
               Up     =  0;

       if (i > 0) {
         if (!widthMin)  {  leftUp = CumulativeMatrix[i-1][minimum_index-1]; }
         if (!widthMax)  { rightUp = CumulativeMatrix[i-1][minimum_index+1]; }
                                Up = CumulativeMatrix[i-1][minimum_index];
         if (!Rotated) {
         if      (leftUp ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index-1] = -1; --minimum_index; }
         else if (    Up ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index]   = -1;                  }
         else  																							  { ValueMatrix[i-1][minimum_index+1] = -1; ++minimum_index; }
        }
         else {
         if      (rightUp ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index+1] = -1; ++minimum_index; }
         else if (    Up ==  std::min({leftUp, rightUp, Up})) { ValueMatrix[i-1][minimum_index]   = -1;                  }
         else                                                 { ValueMatrix[i-1][minimum_index-1] = -1; --minimum_index; }          
         }
      }
  }
}


void Rotate90(std::vector<std::vector<int> > &ValueMatrix)
{
	
  std::vector<std::vector<int>> RotatedMatrix    (ValueMatrix[0].size(),  std::vector<int> (ValueMatrix.size(),   0  ));

  int X_WIDTH = ValueMatrix[0].size();
	int Y_HEIGHT = ValueMatrix.size();
	for(int i=0; i<X_WIDTH; i++) {
	    for(int j=0; j<Y_HEIGHT; j++) {
	        RotatedMatrix[i][j] = ValueMatrix[Y_HEIGHT-1-j][i];
	    }
	}
  ValueMatrix.clear();
  ValueMatrix = RotatedMatrix;
}

void RotateNeg90(std::vector<std::vector<int> > &ValueMatrix)
{
  
  std::vector<std::vector<int>> RotatedMatrix    (ValueMatrix[0].size(),  std::vector<int> (ValueMatrix.size(),   0  ));


  int X_WIDTH = ValueMatrix[0].size();
  int Y_HEIGHT = ValueMatrix.size();
  for(int i=0; i<X_WIDTH; i++) {
      for(int j=0; j<Y_HEIGHT; j++) {
          RotatedMatrix[i][j] = ValueMatrix[j][X_WIDTH - 1 - i];
      }
  }
  ValueMatrix.clear();
  ValueMatrix = RotatedMatrix;
}


void RemoveSeam(int &Y_HEIGHT, int &X_WIDTH, int VERTICAL_SEAMS, std::vector<std::vector<int> > &ValueMatrix, bool Rotated) {
  for (int i=0; i<VERTICAL_SEAMS; ++i) {
    
    std::vector<std::vector<int>> EnergyMatrix     (Y_HEIGHT, std::vector<int> (X_WIDTH,    0  ));
    std::vector<std::vector<int>> CumulativeMatrix (Y_HEIGHT, std::vector<int> (X_WIDTH,    0  ));
    std::vector<std::vector<int>> FinalMatrix      (Y_HEIGHT, std::vector<int> (X_WIDTH-1,  0  ));


    //OutputMatrix(ValueMatrix);

    FillEnergyMatrices (ValueMatrix, EnergyMatrix, CumulativeMatrix);

    FindSeam           (CumulativeMatrix, ValueMatrix, Rotated);

   //OutputMatrix(ValueMatrix);

    FillFinalMatrix(ValueMatrix, FinalMatrix);

    --X_WIDTH;

    ValueMatrix.clear();
    ValueMatrix = FinalMatrix;

  }
}

void OutputFinalMatrix(const std::vector<std::vector<int>> &matrix, std::ofstream &output, int MAX_VALUE) {
  output << "P2\n";
  output << matrix[0].size() << " " << matrix.size() << "\n";
  output << MAX_VALUE << "\n";
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      output << matrix[i][j] << " ";
    }
    if (i != matrix.size()-1) output << "\n";
  }
}





