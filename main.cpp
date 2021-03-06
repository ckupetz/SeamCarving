#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Seam.cpp"


int main(int argc, char * argv[]) {


  //Make sure we have the right number of arguments
  if (argc != 4) { std::cout << "Please use the following format: '[filename] [VERTICAL_SEAMS] [HORIZTONAL_SEAMS]'\n"; return 0; }

  //Initialize our width/height variables, as well as the max B/W value (0-255)
  int MAX_VALUE, X_WIDTH, Y_HEIGHT;
  int VERTICAL_SEAMS =   atoi(argv[2]);
  int HORIZTONAL_SEAMS = atoi(argv[3]);

  //Open the file and load it into a string
  std::ifstream input(argv[1]);
  std::string filename = argv[1];

  //Get values from infile
  GetValuesFromFile(X_WIDTH, Y_HEIGHT, MAX_VALUE, input);

  //Create initial Matrix to read in from infile
  std::vector<std::vector<int>> ValueMatrix      (Y_HEIGHT, std::vector<int> (X_WIDTH,    0  ));

  FillValueMatrix    (ValueMatrix, input);

  RemoveSeam         (Y_HEIGHT, X_WIDTH, VERTICAL_SEAMS, ValueMatrix);

  TransposeMatrix    (ValueMatrix);
  
  std::swap          (X_WIDTH, Y_HEIGHT);

  RemoveSeam         (Y_HEIGHT, X_WIDTH, HORIZTONAL_SEAMS, ValueMatrix);

  TransposeMatrix    (ValueMatrix);

  //Create new filename for outfile
  std::size_t pos = filename.find_last_of(".");
  if (pos != std::string::npos) filename = filename.substr(0, pos) + "_processed" + filename.substr(pos, pos + 2);
  
  //Create the outfile and fill it with the decompressed string
  std::ofstream output(filename);
  OutputFinalMatrix(ValueMatrix, output, MAX_VALUE);
  output.close();
  input.close();

}

