#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <unistd.h>
#include <cstdlib>

#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

using namespace std;

// ================================
// funciones de printeo
// ================================

void showUsageHelp() {
    cout << "💡 Usage: ./super_heart <image_file.txt> [output_file.txt]" << endl;
    cout << "   - image_file.txt: ASCII image input file (required)" << endl;
    cout << "   - output_file.txt: File to save output (optional, if not specified shows on screen)" << endl;
    cout << endl;
    cout << "📋 Examples:" << endl;
    cout << "   ./super_heart imageascii.txt                # Show on screen" << endl;
    cout << "   ./super_heart imageascii.txt result.txt     # Save to file" << endl;
}
void showAvailableFiles() {
    cout << "💡 Available files in current directory:" << endl;
    system("ls -1 *.txt 2>/dev/null || echo '   No .txt files found'");
    cout << endl;
}
void showFileError(const string& filename) {
    cout << "❌ ERROR: Could not open file '" << filename << "'" << endl;
    cout << "🔍 Verify that:" << endl;
    cout << "   - The file exists in current directory" << endl;
    cout << "   - The filename is spelled correctly" << endl;
    cout << "   - You have read permissions for the file" << endl;
    cout << endl;
    showAvailableFiles();
}
void showImageInfo(int width, int height) {
    cout << "✅ Image loaded: " << width << "x" << height << " characters" << endl;
    cout << "🖤 Creating heart adaptive for the image..." << endl << endl;
}

// ================================
// Validar archivos
// ================================

bool validateFileName(const string& filename) {
	return !filename.empty();
}
bool validateImageData(const vector<string>& image) {
	return !image.empty();
}
bool validateOutputFile(const string& outputFile, ofstream& outFile) {
	if (outputFile.empty()) {
		return true;
	}

	outFile.open(outputFile);
	if (!outFile.is_open()) {
		cout << "❌ ERROR: Couldn`t create file '" << outputFile << "'" << endl;
		cout << "🔍 Verify writing permisions for directory" << endl;
		return false;
	}
	return true;
}

// ================================
// FUNCIONES DE ENTRADA/ARGUMENTOS
// ================================

string getImageFileName(int argc, char** argv) {
	if (argc >= 2) {
		return string(argv[1]);
	}
	string filename;
	cout << "📁 Enter ASCII image file: ";
	getline(cin, filename);
	return filename;
}
string getOutputFileName(int argc, char** argv) {
	if (argc >= 3) {
		return string(argv[2]);
	}
	string filename;
	cout << "📝 Enter output file (optional, press Enter to show on screen): ";
	getline(cin, filename);
	return filename;
}

// ================================
// FUNCIONES DE PROCESAMIENTO
// ================================

bool isInsideHeart(float x, float y) {
	// Adaptar el corazon a las dimensiones de la imagen
	// Normalizar las coordenadas para que el corazon cubra toda la imagen
	float nx = (x - 50.0f) / 40.0f;  // Centrar en x=50, escalar para width
	float ny = (40.0f - y) / 35.0f;  // Centrar en y=40, invertir Y, escalar para height
		
	// Ecuacion del corazon: (x^2 + y^2 - 1)^3 - x^2*y^3 <= 0
	float a = nx * nx + ny * ny - 1.0f;
	return (a * a * a - nx * nx * ny * ny * ny) <= 0.0f;
}

vector<string> readASCIIImage(const string& filename) {
	vector<string> image;
	ifstream file(filename);
	string line;

	if (!file.is_open()) {
		showFileError(filename);
		return image; // Vector vacio indica error
	}

	while (getline(file, line)) {
		image.push_back(line);
	}
	file.close();
	return image;
}

void calculateImageDimensions(const vector<string>& image, int& width, int& height) {
	height = image.size();
	width = 0;
	for (const auto& line : image) {
		width = max(width, (int)line.length());
	}
}
char getPixelFromImage(const vector<string>& image, int x, int y) {
	if (y >= (int)image.size() || x >= (int)image[y].length()) {
		return ' ';
	}
	return image[y][x];
}

string formatPixelForOutput(char pixel, bool useColors) {
	if (useColors) {
		return RED + string(1, pixel) + RESET;
	} else {
		return string(1, pixel);
	}
}

void generateHeartOutput(const vector<string>& asciiImage, ofstream& outFile, bool outputToFile) {
	int imageHeight, imageWidth;
	calculateImageDimensions(asciiImage, imageWidth, imageHeight);

	for (int y = 0; y < imageHeight; y++) {
		string line = "";

		for (int x = 0; x < imageWidth; x++) {
			if (isInsideHeart((float)x, (float)y)) {
				// Dentro del corazon: usar caracter de la imagen original
				char pixel = getPixelFromImage(asciiImage, x, y);
				line += formatPixelForOutput(pixel, !outputToFile);
			} else {
				// Fuera del corazon: espacio vacio
				line += " ";
			}
		}
		// Enviar salida al destino apropiado
		if (outputToFile) {
			outFile << line << endl;
		} else {
			cout << line << endl;
		}
	}
}


int main(int argc, char **argv) {
	// Obtener argumentos de entrada
	string imageFile = getImageFileName(argc, argv);
	string outputFile = getOutputFileName(argc, argv);

	// Validar nombre de archivo
	if (!validateFileName(imageFile)) {
		cout << "❌ ERROR: specify valid file." << endl;
		showUsageHelp();
		return 1;
	}

	// Leer imagen ASCII
	vector<string> asciiImage = readASCIIImage(imageFile);

	// Validar que se cargo correctamente
	if (!validateImageData(asciiImage)) {
		cout << "❌ ERROR: could not load ASCII file." << endl;
		cout << "🔄 Try running the program again with a valid file." << endl;
		showUsageHelp();
		return 1;
	}
	// Mostrar informacion de la imagen
	int imageWidth, imageHeight;
	calculateImageDimensions(asciiImage, imageWidth, imageHeight);
	showImageInfo(imageWidth, imageHeight);
	// Configurar archivo de salida
	ofstream outFile;
	bool outputToFile = !outputFile.empty();
	if (!validateOutputFile(outputFile, outFile)) {
		return 1;
	}
	// Generar el súper corazon
	generateHeartOutput(asciiImage, outFile, outputToFile);
	if (outputToFile) {
		outFile.close();
		cout << "✅ Super Heart saved in: " << outputFile << endl;
	}
	return 0;
}