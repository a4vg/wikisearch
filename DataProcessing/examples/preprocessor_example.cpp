// export DATA_DIR=../data/
// g++ --std=c++11 -I../include ../src/Preprocessor.cpp preprocessor_example.cpp

#include <iostream>

#include "Preprocessor.h"

int main()
{
  std::string text1 = "Wikipedia en español es la edición en español de Wikipedia. Al igual que las versiones de Wikipedia que existen en otros idiomas, es una enciclopedia de contenido libre, publicada en Internet bajo las licencias libres CC BY-SA 3.0 y GFDL. En la actualidad cuenta con 1 606 871 artículos, y es escrita por usuarios voluntarios, es decir, que cualquiera puede editar un artículo, corregirlo o ampliarlo. Los servidores son administrados por la Fundación Wikimedia, una organización sin ánimo de lucro cuya financiación se basa fundamentalmente en donaciones.";
  std::string text2 = "Es por ello que este artículo tiene la intención de enseñaros a redactar un texto argumentativo desde el principio para que podáis aplicarlo posteriormente sin tener en cuenta la tesis que vayáis a defender. Si conseguimos que previamente a la redacción el alumno tenga por escrito un guión previo muy definido, la redacción será mucho más fácil. En este sentido el profesor es importante que no sólo valore el texto argumentativo, sino también el borrador o plantilla elaborada por el alumno.";
  
  std::cout << "Loading language files to preprocessor...\n";
  Preprocessor preprocessor(text1); // text can also be set with .setText(string)

  std::cout << "\nWord count for text 1:\n";
  auto wordCount = preprocessor.getWordCount();
  for (const auto& count: wordCount)
    std::cout << count.first << ": " << count.second << "\n";

  preprocessor.setText(text2);

  std::cout << "\nWord count for text 2:\n";
  wordCount = preprocessor.getWordCount();
  for (const auto& count: wordCount)
    std::cout << count.first << ": " << count.second << "\n";
  preprocessor.exportWordCount("output.txt");
}