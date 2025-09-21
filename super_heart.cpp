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
// funciones de print
// ================================

void showUsageHelp() {
	cout << "💡 Uso: ./super_heart <archivo_imagen.txt> [archivo_salida.txt]" << endl;
	cout << "   - archivo_imagen.txt: Imagen ASCII de entrada (obligatorio)" << endl;
	cout << "   - archivo_salida.txt: Archivo donde guardar (opcional, si no se especifica muestra en pantalla)" << endl;
	cout << endl;
	cout << "📋 Ejemplos:" << endl;
	cout << "   ./super_heart imageascii.txt				# Mostrar en pantalla" << endl;
	cout << "   ./super_heart imageascii.txt resultado.txt  # Guardar en archivo" << endl;
}
void showAvailableFiles() {
	cout << "💡 Archivos disponibles en el directorio actual:" << endl;
	system("ls -1 *.txt 2>/dev/null || echo '   No se encontraron archivos .txt'");
	cout << endl;
}
void showFileError(const string& filename) {
	cout << "❌ ERROR: No se pudo abrir el archivo '" << filename << "'" << endl;
	cout << "🔍 Verifica que:" << endl;
	cout << "   - El archivo existe en el directorio actual" << endl;
	cout << "   - El nombre del archivo está escrito correctamente" << endl;
	cout << "   - Tienes permisos de lectura para el archivo" << endl;
	cout << endl;
	showAvailableFiles();
}
void showImageInfo(int width, int height) {
	cout << "✅ Imagen cargada: " << width << "x" << height << " caracteres" << endl;
	cout << "🖤 Creando corazón adaptado a la imagen..." << endl << endl;
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
		return true; // No hay archivo de salida, usar pantalla
	}

	outFile.open(outputFile);
	if (!outFile.is_open()) {
		cout << "❌ ERROR: No se pudo crear el archivo '" << outputFile << "'" << endl;
		cout << "🔍 Verifica permisos de escritura en el directorio" << endl;
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
	cout << "📁 Ingresa el archivo de imagen ASCII: ";
	getline(cin, filename);
	return filename;
}
string getOutputFileName(int argc, char** argv) {
	if (argc >= 3) {
		return string(argv[2]);
	}
	string filename;
	cout << "📝 Ingresa el archivo de salida (opcional, presiona Enter para mostrar en pantalla): ";
	getline(cin, filename);
	return filename;
}

// ================================
// FUNCIONES DE PROCESAMIENTO
// ================================

bool isInsideHeart(float x, float y) {
	// Adaptar el corazón a las dimensiones de la imagen
	// Normalizar las coordenadas para que el corazón cubra toda la imagen
	float nx = (x - 50.0f) / 40.0f;  // Centrar en x=50, escalar para width
	float ny = (40.0f - y) / 35.0f;  // Centrar en y=40, invertir Y, escalar para height
		
	// Ecuación del corazón: (x^2 + y^2 - 1)^3 - x^2*y^3 <= 0
	float a = nx * nx + ny * ny - 1.0f;
	return (a * a * a - nx * nx * ny * ny * ny) <= 0.0f;
}

vector<string> readASCIIImage(const string& filename) {
	vector<string> image;
	ifstream file(filename);
	string line;

	if (!file.is_open()) {
		showFileError(filename);
		return image; // Vector vacío indica error
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
				// Dentro del corazón: usar carácter de la imagen original
				char pixel = getPixelFromImage(asciiImage, x, y);
				line += formatPixelForOutput(pixel, !outputToFile);
			} else {
				// Fuera del corazón: espacio vacío
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
		cout << "❌ ERROR: Debes especificar un archivo válido." << endl;
		showUsageHelp();
		return 1;
	}

	// Leer imagen ASCII
	vector<string> asciiImage = readASCIIImage(imageFile);

	// Validar que se cargó correctamente
	if (!validateImageData(asciiImage)) {
		cout << "❌ ERROR: No se pudo cargar la imagen ASCII." << endl;
		cout << "🔄 Intenta ejecutar el programa nuevamente con un archivo válido." << endl;
		showUsageHelp();
		return 1;
	}
	// Mostrar información de la imagen
	int imageWidth, imageHeight;
	calculateImageDimensions(asciiImage, imageWidth, imageHeight);
	showImageInfo(imageWidth, imageHeight);
	// Configurar archivo de salida
	ofstream outFile;
	bool outputToFile = !outputFile.empty();
	if (!validateOutputFile(outputFile, outFile)) {
		return 1;
	}
	// Generar el súper corazón
	generateHeartOutput(asciiImage, outFile, outputToFile);
	if (outputToFile) {
		outFile.close();
		cout << "✅ Súper corazón guardado en: " << outputFile << endl;
	}
	return 0;
}