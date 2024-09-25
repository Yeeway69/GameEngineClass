#include <iostream>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;


using vec3 = glm::vec3;

// Data Structures
struct Position {
	float x;
	float y;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

struct Triangle {
	Position position;
	Color color;
	float size;
};

// Global list of triangles
std::vector<Triangle> g_triangles;

// Function to read a file's contents into a string
string readFile(const std::string& filename) {
	ifstream fileStream(filename);
	if (!fileStream.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return "";
	}

	string content((istreambuf_iterator<char>(fileStream)),
		istreambuf_iterator<char>());
	fileStream.close();
	return content;
}

// Function to parse JSON string and populate triangles
std::vector<Triangle> parseTriangles(const std::string& jsonString) {
	std::vector<Triangle> triangles;
	try {
		// Parse the JSON string
		json jsonData = json::parse(jsonString);

		// Check if "triangles" key exists and is an array
		if (!jsonData.contains("triangles") || !jsonData["triangles"].is_array()) {
			std::cerr << "JSON does not contain a valid 'triangles' array." << std::endl;
			return triangles;
		}

		// Iterate over each triangle in the JSON
		for (const auto& item : jsonData["triangles"]) {
			Triangle tri;

			// Parse position
			if (item.contains("position") && item["position"].is_object()) {
				tri.position.x = item["position"].value("x", 0.0f);
				tri.position.y = item["position"].value("y", 0.0f);
			}
			else {
				std::cerr << "Triangle missing 'position' data. Skipping this triangle." << std::endl;
				continue; // Skip this triangle
			}

			// Parse color
			if (item.contains("color") && item["color"].is_object()) {
				tri.color.r = static_cast<GLubyte>(item["color"].value("r", 255));
				tri.color.g = static_cast<GLubyte>(item["color"].value("g", 255));
				tri.color.b = static_cast<GLubyte>(item["color"].value("b", 255));
			}
			else {
				std::cerr << "Triangle missing 'color' data. Using default color (white)." << std::endl;
				tri.color = { 255, 255, 255 }; // Default color: white
			}

			// Parse size
			tri.size = item.value("size", 1.0f);

			triangles.push_back(tri);
		}
	}
	catch (json::parse_error& e) {
		std::cerr << "JSON Parse Error: " << e.what() << std::endl;
	}
	catch (json::type_error& e) {
		std::cerr << "JSON Type Error: " << e.what() << std::endl;
	}

	return triangles;
}


void int_opengl() {
	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

// Function to draw a single triangle
void draw_triangle(const Triangle& tri) {
	glColor3ub(tri.color.r, tri.color.g, tri.color.b);

	float halfSize = tri.size / 2.0f;

	// Calculate vertices based on position and size
	float x = tri.position.x;
	float y = tri.position.y;

	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + tri.size);             // Top vertex
	glVertex2f(x - halfSize, y - halfSize);  // Bottom left vertex
	glVertex2f(x + halfSize, y - halfSize);  // Bottom right vertex
	glEnd();
}

void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& tri : g_triangles) {
		draw_triangle(tri);
	}

	glutSwapBuffers();
}

int main(int argc, char *argv[]) {

	// Read JSON data from the file
	std::string jsonString = readFile("triangles.json");
	if (jsonString.empty()) {
		return -1; // Exit if file reading failed
	}

	// Parse the JSON and populate g_triangles
	g_triangles = parseTriangles(jsonString);
	if (g_triangles.empty()) {
		std::cerr << "No triangles to render. Exiting." << std::endl;
		return -1;
	}

	//GLUT Window initialization
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutCreateWindow("OpenGL Triangles!!!");

	//OpenGL initialization
	int_opengl();

	// Set the clear color
	glClearColor(0.2f, 0.5f, 0.7f, 1.0f);

	// Set up the projection (optional, but good practice)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0); // Set coordinate system

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//GLUT callbacks
	glutDisplayFunc(display_func);

	glutMainLoop();


	return 0;
}