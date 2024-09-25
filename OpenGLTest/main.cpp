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
vector<Triangle> g_triangles;

string readFile(const string& filename) {
	ifstream fileStream(filename);
	if (!fileStream.is_open()) {
		cerr << "Failed to open file: " << filename << endl;
		return "";
	}

	string content((istreambuf_iterator<char>(fileStream)),
		istreambuf_iterator<char>());
	fileStream.close();
	return content;
}

// Parse JSON string and populate triangles
vector<Triangle> parseTriangles(const string& jsonString) {
	vector<Triangle> triangles;
	try {

		json jsonData = json::parse(jsonString);

		// Check if "triangles" key is actually a thing
		if (!jsonData.contains("triangles") || !jsonData["triangles"].is_array()) {
			cerr << "JSON has no valid 'triangles' array." << endl;
			return triangles;
		}

		// Iterate over each triangle in the JSON
		for (const auto& item : jsonData["triangles"]) {
			Triangle tri;

			// position
			if (item.contains("position") && item["position"].is_object()) {
				tri.position.x = item["position"].value("x", 0.0f);
				tri.position.y = item["position"].value("y", 0.0f);
			}
			else {
				cerr << "Triangle missing 'position' data. Skipping this triangle." << endl;
				continue; // Skip this triangle
			}

			// color
			if (item.contains("color") && item["color"].is_object()) {
				tri.color.r = static_cast<GLubyte>(item["color"].value("r", 255));
				tri.color.g = static_cast<GLubyte>(item["color"].value("g", 255));
				tri.color.b = static_cast<GLubyte>(item["color"].value("b", 255));
			}
			else {
				cerr << "Triangle missing 'color' data. Using default color (white)." << endl;
				tri.color = { 255, 255, 255 }; // Default color: white
			}

			// size
			tri.size = item.value("size", 1.0f);

			triangles.push_back(tri);
		}
	}
	catch (json::parse_error& e) {
		cerr << "JSON Parse Error: " << e.what() << endl;
	}
	catch (json::type_error& e) {
		cerr << "JSON Type Error: " << e.what() << endl;
	}

	return triangles;
}


void int_opengl() {
	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}


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

	string jsonString = readFile("CrazyTriangles.json");
	if (jsonString.empty()) {
		return -1; // Exit if file reading failed
	}

	g_triangles = parseTriangles(jsonString);
	if (g_triangles.empty()) {
		cerr << "No triangles to render. Exiting." << endl;
		return -1;
	}

	//GLUT Window initialization
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutCreateWindow("OpenGL Triangles!!!");

	int_opengl();

	// background color
	glClearColor(0.0f, 0.4f, 0.5f, 1.0f);

	// Set up the projection
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