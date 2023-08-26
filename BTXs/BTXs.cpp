#include "BTXs\BTXs.h"

#define PI 3.14159265358979323846
using namespace BTXs;

void AddTri(std::vector<vector3>& input, vector3 v1, vector3 v2, vector3 v3)
{
	//3
	//| \
	//1--2
	//This will make the triangle 1->2->3 
	input.push_back(v1);
	input.push_back(v2);
	input.push_back(v3);
}
void AddQuad(std::vector<vector3>& input, vector3 v1, vector3 v2, vector3 v3, vector3 v4)
{
	//3--4
	//|  |
	//1--2
	//This will make the triangle 1->2->3
	input.push_back(v1);
	input.push_back(v2);
	input.push_back(v3);

	//This will make the triangle 4->3->2
	input.push_back(v4);
	input.push_back(v3);
	input.push_back(v2);
}

BTXsDLL std::vector<vector3>
BTXs::GenerateHelix(uint a_uSubdivisions, float a_fRadius1, float a_fRadius2, uint a_nLenght)
{

	if (a_fRadius1 < 0.01f)
		a_fRadius1 = 0.01f;

	if (a_fRadius2 < 0.01f)
		a_fRadius2 = 0.01f;

	if (a_nLenght < 1)
		a_nLenght = 1;

	if (a_uSubdivisions < 3)
		a_uSubdivisions = 3;
	if (a_uSubdivisions > 360)
		a_uSubdivisions = 360;

	std::vector<vector3 > shape;

	float fDelta = 2.0f * PI / static_cast<float>(a_uSubdivisions);
	std::vector<vector3> v3Circle;
	for (uint i = 0; i < a_uSubdivisions; i++)
	{
		vector3 v3Temp(glm::cos(fDelta * i) * a_fRadius1, glm::sin(fDelta * i) * a_fRadius1, 0.0f);
		v3Circle.push_back(v3Temp);
	}

	std::vector<std::vector<vector3>> v3CircleList;
	for (uint j = 0; j < a_uSubdivisions * a_nLenght; j++)
	{
		std::vector<vector3> v3CircleLocal;
		matrix4 m4Temp = glm::rotate(fDelta * j, vector3(0.0f, 1.0f, 0.0f));
		m4Temp = m4Temp * glm::translate(vector3(a_fRadius2, (a_fRadius1 * j), 0.0f));

		for (uint i = 0; i < a_uSubdivisions; i++)
		{
			v3CircleLocal.push_back(m4Temp * vector4(v3Circle[i], 1.0f));
		}
		v3CircleList.push_back(v3CircleLocal);
	}

	for (uint j = 0; j < (a_uSubdivisions * a_nLenght) - 1; j++)
	{
		for (uint i = 0; i < a_uSubdivisions; i++)
		{
			uint iLoop = (i + 1) % a_uSubdivisions;
			uint jLoop = (j + 1) % (a_uSubdivisions * a_nLenght);
			AddQuad(shape, v3CircleList[j][i], v3CircleList[jLoop][i], v3CircleList[j][iLoop], v3CircleList[jLoop][iLoop]);
		}
	}
	return shape;
}

BTXsDLL std::vector<vector3> 
BTXs::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	std::vector<vector3 > shape;
	
	std::vector<vector3 > vertex;
	vector3 bottom = vector3(0.0f, -a_fHeight / 2.0f, 0.0f);
	vector3 top = vector3(0.0f, a_fHeight / 2.0f, 0.0f);

	GLfloat theta = 0.0f;
	float fDelta = static_cast<GLfloat>(2 * PI) / static_cast<GLfloat>(a_nSubdivisions);
	//Calculating the circle vertices which will be used for the bottom cover
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fRadius,
			-a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fRadius);
		vertex.push_back(temp);
		theta += fDelta;
	}
	//Create the final positions for each side face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(shape, top, vertex[i], vertex[(i + 1) % a_nSubdivisions]);
	}
	//Create the final positions for each bottom face, the order needs to be
	//changed because otherwise the faces would be facing inward
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(shape, bottom, vertex[(i + 1) % a_nSubdivisions], vertex[i]);
	}
	

	return shape;
}

BTXsDLL std::vector<vector3> 
BTXs::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	std::vector<vector3 > shape;

	
	vector3 bottom = vector3(0.0f, -a_fHeight / 2.0f, 0.0f);
	vector3 top = vector3(0.0f, a_fHeight / 2.0f, 0.0f);

	GLfloat theta = 0.0f;
	float fDelta = static_cast<GLfloat>(2 * PI) / static_cast<GLfloat>(a_nSubdivisions);
	std::vector<vector3 > vertexTop;
	//Calculating the circle vertices which will be used for the bottom cover
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fRadius,
			a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fRadius);
		vertexTop.push_back(temp);
		theta += fDelta;
	}
	std::vector<vector3 > vertexBottom;
	//Calculating the circle vertices which will be used for the top cover
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vertexTop[i] - vector3(0.0f, a_fHeight, 0.0f);
		vertexBottom.push_back(temp);
	}

	//Create the final positions for each Top face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(shape, top, vertexTop[i], vertexTop[(i + 1) % a_nSubdivisions]);
	}
	//Create the final positions for each Bottom face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(shape, bottom, vertexBottom[(i + 1) % a_nSubdivisions], vertexBottom[i]);
	}
	//Create the final positions for each Side face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(shape,	vertexBottom[i], vertexBottom[(i+1) % a_nSubdivisions], 
						vertexTop[i], vertexTop[(i + 1) % a_nSubdivisions]);
	}

	return shape;
}

BTXsDLL std::vector<vector3> 
BTXs::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	std::vector<vector3 > shape;

	//Calculating the vertices for the 4 circles
	std::vector<vector3 > v3OuterCircleTop;
	GLfloat theta = 0;
	float fDelta = static_cast<GLfloat>(2 * PI) / static_cast<GLfloat>(a_nSubdivisions);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fOuterRadius,
			a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fOuterRadius);
		v3OuterCircleTop.push_back(temp);
		theta += fDelta;
	}
	std::vector<vector3 > v3InnerCircleTop;
	theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fInnerRadius,
			a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fInnerRadius);
		v3InnerCircleTop.push_back(temp);
		theta += fDelta;
	}
	std::vector<vector3 > v3OuterCircleBottom;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = v3OuterCircleTop[i] - vector3(0.0, a_fHeight, 0.0f);
		v3OuterCircleBottom.push_back(temp);
	}
	std::vector<vector3 > v3InnerCircleBottom;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp = v3InnerCircleTop[i] - vector3(0.0, a_fHeight, 0.0f);
		v3InnerCircleBottom.push_back(temp);
	}
	//Generate shapes
	//Top Circle
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(shape,
			v3OuterCircleTop[i], v3OuterCircleTop[(i + 1) % a_nSubdivisions],
			v3InnerCircleTop[i], v3InnerCircleTop[(i + 1) % a_nSubdivisions]);
	}
	//Bottom Circle
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(shape,
			v3InnerCircleBottom[i], v3InnerCircleBottom[(i + 1) % a_nSubdivisions],
			v3OuterCircleBottom[i], v3OuterCircleBottom[(i + 1) % a_nSubdivisions]);
	}
	//Outer Faces
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(shape,
			v3OuterCircleBottom[i], v3OuterCircleBottom[(i + 1) % a_nSubdivisions],
			v3OuterCircleTop[i], v3OuterCircleTop[(i + 1) % a_nSubdivisions]);
	}
	//Inner Faces
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(shape,
			v3InnerCircleTop[i], v3InnerCircleTop[(i + 1) % a_nSubdivisions],
			v3InnerCircleBottom[i], v3InnerCircleBottom[(i + 1) % a_nSubdivisions]);
	}
	
	return shape;
}

BTXsDLL std::vector<vector3> 
BTXs::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsHeight, int a_nSubdivisionsAxis)
{
	//Checking for Minimal Values
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsHeight < 3)
		a_nSubdivisionsHeight = 3;
	if (a_nSubdivisionsHeight > 360)
		a_nSubdivisionsHeight = 360;

	if (a_nSubdivisionsAxis < 3)
		a_nSubdivisionsAxis = 3;
	if (a_nSubdivisionsAxis > 360)
		a_nSubdivisionsAxis = 360;

	std::vector<vector3 > shape;

	/*
		We will calculate a circle first, then move it to the right position and orientation for each cross section
		of a circle in the torus but we will not create a circle mesh, just the points then add by index each cross section
		with the next
	*/

	// The torus will use two radii first outer and inner will need to get subtracted so we get the diameter
	// outer is distance from world center to outermost point of the circle cross-section and inner is from center 
	// to closest point in circle cross-section
	float fRadius = (glm::distance(vector3(a_fOuterRadius, 0.0f, 0.0f), vector3(a_fInnerRadius, 0.0f, 0.0f))) / 2.0f;

	//Calculate a single circle's vertices first
	std::vector<vector3> circleVertices;

	//Start at 0 and then check increments on the resolution of the circle cross-sections
	GLfloat theta = 0;
	GLfloat delta = static_cast<GLfloat>(2 * PI / static_cast<float>(a_nSubdivisionsHeight));

	for (int i = 0; i < a_nSubdivisionsHeight; i++)
	{
		//calculate the point and push it to the list then go for the next one
		vector3 circlePoint = vector3(cos(theta) * fRadius, sin(theta) * fRadius, 0.0f);
		circleVertices.push_back(circlePoint);
		theta += delta;
	}

	//Calculate the position of the cross sections, we already have the circle vertices just move
	//them to the right orientation and position
	//Calculate the circle cross sections or the number of circles that you can cut from the torus

	static float fCrossSectionAngle = 0;
	float fCrossSectionDelta = 360.0f / static_cast<float>(a_nSubdivisionsAxis);
	//The center of the cross section is the first point I reach from the center plus the radius
	float fCrossSectionRadius = a_fOuterRadius + fRadius;
	//This is a list of "lists of circle points"
	std::vector<std::vector<vector3>> listOfCircles; //container for each cross section (list of lists of vertices)

	for (uint j = 0; j < static_cast<uint>(a_nSubdivisionsAxis); j++)
	{
		theta = 0; //reset the angle
		std::vector<vector3> crossSection; //container of each circle cross section vertex (list of vertices)

		//Make a copy of the points and move them to the right orientation and place
		for (int i = 0; i < a_nSubdivisionsHeight; i++)
		{
			vector3 vertex = circleVertices[i];
			matrix4 circleSpace = glm::rotate(matrix4(), glm::radians(fCrossSectionAngle), vector3(0.0f, 1.0f, 0.0f));
			circleSpace = glm::translate(circleSpace, vector3(fCrossSectionRadius, 0.0f, 0.0f));
			vertex = circleSpace * vector4(vertex, 1.0f); //bring them from local space to global
			crossSection.push_back(vertex);
		}
		listOfCircles.push_back(crossSection); //Push the circle cross section to the list
		fCrossSectionAngle += fCrossSectionDelta; //Go for the next angle by adding the current plus the increment
	}

	//Create the faces each horizontal ring at a time (can be each vertical ring at a time as well)
	for (uint j = 0; j < static_cast<uint>(a_nSubdivisionsAxis); j++)
	{
		for (uint i = 0; i < static_cast<uint>(a_nSubdivisionsHeight); i++)
		{
			vector3 v3BottomLeft = listOfCircles[(i + 0)][j];
			vector3 v3BottomRight = listOfCircles[(i + 1) % a_nSubdivisionsAxis][j];

			vector3 v3TopLeft = listOfCircles[(i + 0)][(j + 1) % a_nSubdivisionsHeight];
			vector3 v3TopRight = listOfCircles[(i + 1) % a_nSubdivisionsHeight][(j + 1) % a_nSubdivisionsAxis];

			AddQuad( shape, v3BottomLeft, v3BottomRight, v3TopLeft, v3TopRight);
		}
	}
	return shape;
}

BTXsDLL std::vector<vector3>
BTXs::GenerateSphere(float a_fRadius, int a_nSubdivisions)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_nSubdivisions < 5)
		a_nSubdivisions = 5;

	if (a_nSubdivisions > 20)
		a_nSubdivisions = 20;

	//a_fRadius /= 2.0f;

	std::vector<vector3 > shape;
	// Create the top and bottom center points
	float fHeight = a_fRadius;
	vector3 centerTop(0, fHeight, 0);
	vector3 centerBottom(0, -fHeight, 0);

	// Loop to generate the sphere
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Find the angle that we are using to make these points
		float angle1 = 2 * glm::pi<float>() * (float)i / a_nSubdivisions;
		float angle2 = 2 * glm::pi<float>() * (float)(i + 1) / a_nSubdivisions;

		// Make two vectors to hold the points for this section and the next section of the sphere
		std::vector<vector3> points1;
		std::vector<vector3> points2;

		// Loop to create the points for this and the next section of the sphere
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			points1.push_back( vector3(
				sin(glm::pi<float>() * j / (float)a_nSubdivisions) * a_fRadius * cos(angle1), 
				cos(glm::pi<float>() * (j / (float)a_nSubdivisions)) * fHeight, 
				sin(glm::pi<float>() * j / (float)a_nSubdivisions) * a_fRadius * sin(angle1)));
		}
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// We do the same thing as we did for the last section, but using angle2 instead
			points2.push_back( vector3(
				sin(glm::pi<float>() * j / (float)a_nSubdivisions) * a_fRadius * cos(angle2), 
				cos(glm::pi<float>() * (j / (float)a_nSubdivisions)) * fHeight, 
				sin(glm::pi<float>() * j / (float)a_nSubdivisions) * a_fRadius * sin(angle2)));
		}

		// Create quads and triangles
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// If we are at the last index, we will make a tri connecting to the bottom of the sphere
			if (j == a_nSubdivisions - 1)
			{
				AddTri( shape, points1[j], points2[j], centerBottom);
			}

			// Otherwise, we will create a quad using these four points
			else
			{
				AddQuad(shape, points1[j + 1], points1[j], points2[j + 1], points2[j]);
			}

			// If we are at the first index, we will also create a tri connecting to the top of the sphere
			if (j == 0)
			{
				AddTri(shape, points2[j], points1[j], centerTop);
			}
		}
	}
	return shape;
}

BTXsDLL std::vector<vector3>
BTXs::GenerateIcosahedron(float a_fRadius, int a_nSubdivisions)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
		a_nSubdivisions = 1;

	if (a_nSubdivisions > 5)
		a_nSubdivisions = 5;

	std::vector<vector3 > shape;

	// Replace this with your code
	//Hold values used in initalizing icosahedron
	GLfloat t = sin(1.0f);
	GLfloat y = cos(1.0f);

	//Inital points
	std::vector<vector3> points =
	{
		glm::normalize(vector3(-y, t, 0.0)),
		glm::normalize(vector3(y, t, 0.0)),
		glm::normalize(vector3(-y,-t, 0.0)),
		glm::normalize(vector3(y,-t, 0.0)),

		glm::normalize(vector3(0.0,-y, t)),
		glm::normalize(vector3(0.0, y, t)),
		glm::normalize(vector3(0.0,-y,-t)),
		glm::normalize(vector3(0.0, y,-t)),

		glm::normalize(vector3(t, 0.0,-y)),
		glm::normalize(vector3(t, 0.0, y)),
		glm::normalize(vector3(-t, 0.0,-y)),
		glm::normalize(vector3(-t, 0.0, y))
	};

	//initial indicies
	std::vector<GLint> indicies = { 0, 11,  5,  0,  5,  1,  0,  1,  7,  0,  7, 10,  0, 10, 11,
		1,  5,  9,  5, 11,  4, 11, 10,  2, 10,  7,  6,  7,  1,  8,
		3,  9,  4,  3,  4,  2,  3,  2,  6,  3,  6,  8,  3,  8,  9,
		4,  9,  5,  2,  4, 11,  6,  2, 10,  8,  6,  7,  9,  8,  1 };

	//Temp Vector for midpoints
	vector3 t12;
	vector3 t23;
	vector3 t31;

	//Loop for the amount of subdivisions
	for (GLint i = 0; i < a_nSubdivisions; i++)
	{
		std::vector<GLint> newIndicies; //Holds the new indicies

		//Loop for the amount of indicies but increase by 3 since 3 indicies are used for 1 tri
		for (GLuint j = 0; j < indicies.size(); j += 3)
		{
			//Get the midpoints between each of the three vectors that make up
			t12 = (points[indicies[j]] + points[indicies[j + 1]]) / 2.0f;
			t23 = (points[indicies[j + 1]] + points[indicies[j + 2]]) / 2.0f;
			t31 = (points[indicies[j + 2]] + points[indicies[j]]) / 2.0f;

			//Normalize the midpoints
			t12 = glm::normalize(t12);
			t23 = glm::normalize(t23);
			t31 = glm::normalize(t31);

			//Add the midpoints to the list of points
			points.push_back(t12); //Size - 3
			points.push_back(t23); //Size - 2
			points.push_back(t31); //Size - 1

			//Push back the new indicies for the triangles
			newIndicies.push_back(indicies[j]);
			newIndicies.push_back(points.size() - 3);
			newIndicies.push_back(points.size() - 1);

			newIndicies.push_back(indicies[j + 1]);
			newIndicies.push_back(points.size() - 2);
			newIndicies.push_back(points.size() - 3);

			newIndicies.push_back(indicies[j + 2]);
			newIndicies.push_back(points.size() - 1);
			newIndicies.push_back(points.size() - 2);

			newIndicies.push_back(points.size() - 3);
			newIndicies.push_back(points.size() - 2);
			newIndicies.push_back(points.size() - 1);
		}
		indicies = newIndicies; //Set the indicies to the new indicies
	}

	//Add the triangles
	for (GLuint i = 0; i < indicies.size(); i += 3)
	{
		AddTri( shape, points[indicies[i]], points[indicies[i + 1]], points[indicies[i + 2]]);
	}

	//Scale points
	for (GLuint i = 0; i < indicies.size(); ++i)
	{
		shape[i] = vector3(glm::scale(vector3(a_fRadius)) * vector4(shape[i], 1));
	}

	return shape;
}

BTXsDLL std::vector<vector3>
BTXs::GenerateStar(uint a_uSubdivisions, float fRadius1, float fRadius2)
{
	std::vector<vector3 > shape;

	a_uSubdivisions = std::max(static_cast<uint>(3), a_uSubdivisions);
	a_uSubdivisions = std::min(static_cast<uint>(15), a_uSubdivisions);
	fRadius1 = std::max(0.1f, fRadius1);
	fRadius2 = std::max(0.1f, fRadius2);

	float fDelta = static_cast<float>(2.0f * PI / static_cast<float>(a_uSubdivisions));
	float fDeltaHalf = fDelta / 2.0f;
	std::vector<vector3> vertex1;
	for (uint i = 0; i < a_uSubdivisions; ++i)
	{
		vector3 v3Temp(glm::cos(fDelta * i) * fRadius1, glm::sin(fDelta * i) * fRadius1, 0.0f);
		vertex1.push_back(v3Temp);
	}
	std::vector<vector3> vertex2;
	for (uint i = 0; i < a_uSubdivisions; ++i)
	{
		vector3 v3Temp(glm::cos(fDelta * i + fDeltaHalf) * fRadius2, glm::sin(fDelta * i + fDeltaHalf) * fRadius2, 0.0f);
		vertex2.push_back(v3Temp);
	}
	vector3 v3Front = vector3(0.0f, 0.0f, fRadius2);
	vector3 v3Back = vector3(0.0f, 0.0f, -fRadius2);
	for (uint i = 0; i < a_uSubdivisions; ++i)
	{
		//Front faces
		AddTri(shape, v3Front, vertex1[i], vertex2[i]);
		AddTri(shape, v3Front, vertex2[i], vertex1[(i + 1) % a_uSubdivisions]);
		//Back faces
		AddTri(shape, v3Back, vertex2[i], vertex1[i]);
		AddTri(shape, v3Back, vertex1[(i + 1) % a_uSubdivisions], vertex2[i]);
	}
	return shape;
}

BTXsDLL eSATResults BTXs::ComputeSAT(matrix4 mToWorldA, vector3 v3HalfWidthA, vector3 v3CenterGlobalA,
	matrix4 mToWorldB, vector3 v3HalfWidthB, vector3 v3CenterGlobalB)
{
	//Extract the transformation out of the matrix
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mToWorldA, scale, rotation, translation, skew, perspective);
	//Remove the scale component out of the matrix
	mToWorldA = mToWorldA * glm::scale(1.0f / scale);
	v3HalfWidthA = scale * v3HalfWidthA;
	glm::decompose(mToWorldB, scale, rotation, translation, skew, perspective);
	//Remove the scale component out of the matrix
	mToWorldB = mToWorldB * glm::scale(1.0f / scale);
	v3HalfWidthB = scale * v3HalfWidthB;

	// Get the rotation out of the matrix A
	vector3 v3RotationA[3];
	v3RotationA[0] = vector3(mToWorldA[0][0], mToWorldA[0][1], mToWorldA[0][2]);
	v3RotationA[1] = vector3(mToWorldA[1][0], mToWorldA[1][1], mToWorldA[1][2]);
	v3RotationA[2] = vector3(mToWorldA[2][0], mToWorldA[2][1], mToWorldA[2][2]);

	//Get the rotation out of the matrix B
	vector3 v3RotationB[3];
	v3RotationB[0] = vector3(mToWorldB[0][0], mToWorldB[0][1], mToWorldB[0][2]);
	v3RotationB[1] = vector3(mToWorldB[1][0], mToWorldB[1][1], mToWorldB[1][2]);
	v3RotationB[2] = vector3(mToWorldB[2][0], mToWorldB[2][1], mToWorldB[2][2]);

	// Compute rotation matrix expressing b in a's coordinate frame
	float fCenterAToMiddle, fCenterBToMiddle;
	glm::mat3 m3Rotation, m3RotationAbs;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3Rotation[i][j] = glm::dot(v3RotationA[i], v3RotationB[j]);

	// Compute translation vector v3Distance (this is the distance between both centers in global space)
	vector3 v3Distance = v3CenterGlobalB - v3CenterGlobalA;

	// Bring translation into A's coordinate frame
	v3Distance = vector3(glm::dot(v3Distance, v3RotationA[0]), glm::dot(v3Distance, v3RotationA[1]), glm::dot(v3Distance, v3RotationA[2])); //distance in A's local
	// their cross product is (near) null (see the orange book for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m3RotationAbs[i][j] = std::abs(m3Rotation[i][j]) + 0.0001f;

	// Test axis AX
	fCenterAToMiddle = v3HalfWidthA.x;
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[0][0] + v3HalfWidthB.y * m3RotationAbs[0][1] + v3HalfWidthB.z * m3RotationAbs[0][2];
	if (std::abs(v3Distance.x) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AX;
	}

	// Test axis AY
	fCenterAToMiddle = v3HalfWidthA.y;
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[1][0] + v3HalfWidthB.y * m3RotationAbs[1][1] + v3HalfWidthB.z * m3RotationAbs[1][2];
	if (std::abs(v3Distance.y) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AY;
	}

	// Test axis AZ
	fCenterAToMiddle = v3HalfWidthA.z;
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[2][0] + v3HalfWidthB.y * m3RotationAbs[2][1] + v3HalfWidthB.z * m3RotationAbs[2][2];
	if (std::abs(v3Distance.z) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AZ;
	}

	// Test axis BX
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[0][0] + v3HalfWidthA.y * m3RotationAbs[1][0] + v3HalfWidthA.z * m3RotationAbs[2][0];
	fCenterBToMiddle = v3HalfWidthB.x;
	if (std::abs(v3Distance.x * m3Rotation[0][0] + v3Distance.y * m3Rotation[1][0] + v3Distance.z * m3Rotation[2][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_BX;
	}

	// Test axis BY
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[0][1] + v3HalfWidthA.y * m3RotationAbs[1][1] + v3HalfWidthA.z * m3RotationAbs[2][1];
	fCenterBToMiddle = v3HalfWidthB.y;
	if (std::abs(v3Distance.x * m3Rotation[0][1] + v3Distance.y * m3Rotation[1][1] + v3Distance.z * m3Rotation[2][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_BY;
	}

	// Test axis BZ
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[0][2] + v3HalfWidthA.y * m3RotationAbs[1][2] + v3HalfWidthA.z * m3RotationAbs[2][2];
	fCenterBToMiddle = v3HalfWidthB.z;
	if (std::abs(v3Distance.x * m3Rotation[0][2] + v3Distance.y * m3Rotation[1][2] + v3Distance.z * m3Rotation[2][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_BZ;
	}

	// Test axis AX x BX
	fCenterAToMiddle = v3HalfWidthA.y * m3RotationAbs[2][0] + v3HalfWidthA.z * m3RotationAbs[1][0];
	fCenterBToMiddle = v3HalfWidthB.y * m3RotationAbs[0][2] + v3HalfWidthB.z * m3RotationAbs[0][1];
	if (std::abs(v3Distance.z * m3Rotation[1][0] - v3Distance.y * m3Rotation[2][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AXxBX;
	}

	// Test axis AX x BY
	fCenterAToMiddle = v3HalfWidthA.y * m3RotationAbs[2][1] + v3HalfWidthA.z * m3RotationAbs[1][1];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[0][2] + v3HalfWidthB.z * m3RotationAbs[0][0];
	if (std::abs(v3Distance.z * m3Rotation[1][1] - v3Distance.y * m3Rotation[2][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AXxBY;
	}

	// Test axis AX x BZ
	fCenterAToMiddle = v3HalfWidthA.y * m3RotationAbs[2][2] + v3HalfWidthA.z * m3RotationAbs[1][2];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[0][1] + v3HalfWidthB.y * m3RotationAbs[0][0];
	if (std::abs(v3Distance.z * m3Rotation[1][2] - v3Distance.y * m3Rotation[2][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AXxBZ;
	}

	// Test axis AY x BX
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[2][0] + v3HalfWidthA.z * m3RotationAbs[0][0];
	fCenterBToMiddle = v3HalfWidthB.y * m3RotationAbs[1][2] + v3HalfWidthB.z * m3RotationAbs[1][1];
	if (std::abs(v3Distance.x * m3Rotation[2][0] - v3Distance.z * m3Rotation[0][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AYxBX;
	}

	// Test axis AY x BY
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[2][1] + v3HalfWidthA.z * m3RotationAbs[0][1];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[1][2] + v3HalfWidthB.z * m3RotationAbs[1][0];
	if (std::abs(v3Distance.x * m3Rotation[2][1] - v3Distance.z * m3Rotation[0][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AYxBY;
	}

	// Test axis AY x BZ
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[2][2] + v3HalfWidthA.z * m3RotationAbs[0][2];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[1][1] + v3HalfWidthB.y * m3RotationAbs[1][0];
	if (std::abs(v3Distance.x * m3Rotation[2][2] - v3Distance.z * m3Rotation[0][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AYxBZ;
	}

	// Test axis AZ x BX
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[1][0] + v3HalfWidthA.y * m3RotationAbs[0][0];
	fCenterBToMiddle = v3HalfWidthB.y * m3RotationAbs[2][2] + v3HalfWidthB.z * m3RotationAbs[2][1];
	if (std::abs(v3Distance.y * m3Rotation[0][0] - v3Distance.x * m3Rotation[1][0]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AZxBX;
	}

	// Test axis AZ x BY
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[1][1] + v3HalfWidthA.y * m3RotationAbs[0][1];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[2][2] + v3HalfWidthB.z * m3RotationAbs[2][0];
	if (std::abs(v3Distance.y * m3Rotation[0][1] - v3Distance.x * m3Rotation[1][1]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AZxBY;
	}

	// Test axis AZ x BZ
	fCenterAToMiddle = v3HalfWidthA.x * m3RotationAbs[1][2] + v3HalfWidthA.y * m3RotationAbs[0][2];
	fCenterBToMiddle = v3HalfWidthB.x * m3RotationAbs[2][1] + v3HalfWidthB.y * m3RotationAbs[2][0];
	if (std::abs(v3Distance.y * m3Rotation[0][2] - v3Distance.x * m3Rotation[1][2]) > fCenterAToMiddle + fCenterBToMiddle)
	{
		return BTXs::eSATResults::SAT_AZxBZ;
	}

	// Since no separating axis found, the OBBs must be intersecting
	return BTXs::eSATResults::SAT_NONE;
}