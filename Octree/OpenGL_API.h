#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Linear_Algebra.h"
#include "Camera.h"
#include "OpenGL_Render.h"
#include "ObjFile.h"
#include "Octree.h"

namespace OpenGL_API
{
	/* CAMERA */
	Camera cam = Camera({ 10.0f, 5.0f, 10.0f, 1.0f },	// viewpoint
	{ 0.0f, 0.0f, 0.0f, 1.0f },		// lookat
	{ 0.0f, 1.0f, 0.0f, 0.0f });	// view up

	//LIGHT
	GLfloat posicaoLuz[4] = { 10.0f, 10.0f, 10.0f, 1.0f };

	// mouse event handlers
	Vector4Df last = { 0.0f, 0.0f, 0.0f, 0.0f };
	int TheButtonState = 0;
	int TheKeyState = 0;
	float panSpeed = 0.05f;

	// keyboard event handlers
	int index = 0; //to select the current tree
	char RotateAxis = 'Y';
	bool MoveKey = false;
	bool RotateKey = false;
	bool ScaleKey = false;

	// Class ObjFile
	ObjFile inputObjFile;
	// Class Octree
	Octree objOctree(5); /* int = octree deep */

	/* ======================================= INITIALIZE ======================================= */

	void init(void)
	{
		/* LIGHT */
		
		GLfloat luzAmbiente[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Ambient Light
		GLfloat luzDifusa[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // "cor" 
		GLfloat luzEspecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // "brilho" 
		GLfloat especularidade[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // brilho do material
		GLint especMaterial = 100;

		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade); // refletância do material 
		glMateriali(GL_FRONT, GL_SHININESS, especMaterial); // concentração do brilho

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente); // Ambient Light ENABLE
		// light0 Parameters
		glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
		glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
		glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

		glEnable(GL_COLOR_MATERIAL); // definição da cor do material a partir da cor corrente
		glEnable(GL_LIGHTING); // lighting ENABLE
		glEnable(GL_LIGHT0); // light0 ENABLE
		//glEnable(GL_DEPTH_TEST); // depth-buffering ENABLE
		
		/* LIGHT END */

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.3, 0.3, 0.3, 0.0); // Especifica a cor de fundo da janela
		glShadeModel(GL_SMOOTH); // Habilita o modelo de colorização de Gouraud
		glEnable(GL_LINE_SMOOTH);
		/*glEnable(GL_DEPTH_TEST);*/
		//glEnable(GL_CULL_FACE); //  remove as faces cujas normais apóntam para o semiespaço oposto ao do observador
	}

	/* ======================================= glutDisplayFunc ======================================= */

	void display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, cam.lookat.x, cam.lookat.y, cam.lookat.z, cam.up.x, cam.up.y, cam.up.z);

		// grid
		glBegin(GL_LINES);
		for (float i = -100; i <= 100; ++i)
		{
			glColor4f(0.3f, 0.3f, 0.3f, 0.5f * (1 - abs(i) / 100));
			if (i == 0) glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glVertex3f(i, 0.0f, -100.0f);
			glVertex3f(i, 0.0f, 100.0f);
			if (i == 0) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glVertex3f(100.0f, 0.0f, i);
			glVertex3f(-100.0f, 0.0f, i);
		}
		glEnd();

		// Y-axis
		/*glBegin(GL_LINES);
		glColor4f(0.0f, 0.0f, 0.5f, 0.8f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		glEnd();*/

		OpenGL_Render::LoadObj(inputObjFile);
		OpenGL_Render::LoadOct(objOctree, index);

		// light
		glPushMatrix();
		glTranslatef(posicaoLuz[0], posicaoLuz[1], posicaoLuz[2]);
		glutWireSphere(0.2, 20, 16); //radius, slices, stacks
		glPopMatrix();

		//glFlush();
		glutSwapBuffers();
	}

	/* ======================================= glutReshapeFunc ======================================= */

	void reshape(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 4.0f, 500.0);
		glMatrixMode(GL_MODELVIEW);
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuModify(int op)
	{
		switch (op) {
		case 0:
			// Move
			MoveKey = true;
			RotateKey = false;
			ScaleKey = false;
			cout << "Move ON" << endl;
			break;
		case 1:
			// Rotate
			RotateKey = true;
			MoveKey = false;
			ScaleKey = false;
			cout << "Rotate ON" << endl;
			break;
		case 2:
			// Scale
			ScaleKey = true;
			MoveKey = false;
			RotateKey = false;
			cout << "Scale ON" << endl;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuEditing(int op)
	{
		switch (op) {
		case 0:
			// Union
			if (objOctree.viewEnable == true && objOctree.treeList.size() >= 2)
				objOctree.Union();
			index = 0;
			break;
		case 1:
			// Subtract
			if (objOctree.viewEnable == true && objOctree.treeList.size() >= 2)
				objOctree.Subtract();
			index = 0;
			break;
		case 2:
			// Intersect
			if (objOctree.viewEnable == true && objOctree.treeList.size() >= 2)
				objOctree.Intersect();
			index = 0;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuModeling(int op)
	{
		switch (op) {
		case 0:
			objOctree.Box({ 0.0f, 2.0f, 0.0f, 1.0f }, 2.0f, 3.0f, 4.0f); //corner, sizeX, sizeY, sizeZ
			inputObjFile.viewEnable = false;
			objOctree.viewEnable = true;
			break;
		case 1:
			objOctree.Sphere({ 0.0f, 2.0f, 0.0f, 1.0f }, 2.0f); //center, radius
			inputObjFile.viewEnable = false;
			objOctree.viewEnable = true;
			break;
		case 2:
			objOctree.Cylinder({ 0.0f, 2.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, 1.0f, 5.0f); //center, direction, radius, height
			inputObjFile.viewEnable = false;
			objOctree.viewEnable = true;
			break;
		case 3:
			objOctree.Cone({ 0.0f, 2.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, 1.5f, 5.0f); //center, direction, radius, height
			inputObjFile.viewEnable = false;
			objOctree.viewEnable = true;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuOBJ(int op)
	{
		switch (op) {
		case 0:
			if (inputObjFile.ReadObjFile() == false) {
				MessageBox(NULL, "File could not be opened!", "Info",
					MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				inputObjFile.viewEnable = true;
				objOctree.viewEnable = false;
			}
			break;
		case 1:
			if (inputObjFile.viewEnable == true)
			{
				objOctree.Solid(inputObjFile.objectList);
				inputObjFile.viewEnable = false;
				objOctree.viewEnable = true;
			}
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuTree(int op)
	{
		switch (op) {
		case 0:
			if (objOctree.ReadTreeFile() == false) {
				MessageBox(NULL, "File could not be opened!", "Info",
					MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				objOctree.viewEnable = true;
				inputObjFile.viewEnable = false;
			}
			break;
		case 1:
			if (objOctree.viewEnable == true)
				objOctree.WriteTreeFile();
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuMassProperty(int op)
	{
		switch (op) {
		case 0:
			if (objOctree.viewEnable == true)
			{
				MessageBox(NULL, objOctree.Volume().c_str(), "Mass Property",
					MB_OK | NULL);
			}
			break;
		case 1:
			if (objOctree.viewEnable == true)
			{
				MessageBox(NULL, objOctree.Area().c_str(), "Mass Property",
					MB_OK | NULL);
			}
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void ProcessMenu(int op)
	{
		switch (op) {
		case 0:
			inputObjFile.objectList.clear();
			inputObjFile.viewEnable = false;
			objOctree.treeList.clear();
			objOctree.viewEnable = false;
			break;
		case 1:
			glutDestroyWindow(1); //exit
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= MAIN MENU ======================================= */

	void MainMenu()
	{
		int subMenu1, subMenu2, subMenu3, subMenu4, subMenu5, subMenu6;

		subMenu1 = glutCreateMenu(MenuModeling);
		glutAddMenuEntry("Box", 0);
		glutAddMenuEntry("Sphere", 1);
		glutAddMenuEntry("Cylinder", 2);
		glutAddMenuEntry("Cone", 3);

		subMenu2 = glutCreateMenu(MenuOBJ);
		glutAddMenuEntry("Load", 0);
		glutAddMenuEntry("Octree", 1);

		subMenu3 = glutCreateMenu(MenuEditing);
		glutAddMenuEntry("Union", 0);
		glutAddMenuEntry("Subtract", 1);
		glutAddMenuEntry("Intersect", 2);

		subMenu4 = glutCreateMenu(MenuModify);
		glutAddMenuEntry("Move", 0);
		glutAddMenuEntry("Rotate", 1);
		glutAddMenuEntry("Scale", 2);

		subMenu5 = glutCreateMenu(MenuMassProperty);
		glutAddMenuEntry("Volume", 0);
		glutAddMenuEntry("Area", 1);

		subMenu6 = glutCreateMenu(MenuTree);
		glutAddMenuEntry("Load", 0);
		glutAddMenuEntry("Export", 1);

		glutCreateMenu(ProcessMenu);
		glutAddMenuEntry("NEW", 0);
		glutAddSubMenu("MODELING", subMenu1);
		glutAddSubMenu("OBJ FILE", subMenu2);
		glutAddSubMenu("SOLID EDITING", subMenu3);
		glutAddSubMenu("MODIFY", subMenu4);
		glutAddSubMenu("MASS PROPERTY", subMenu5);
		glutAddSubMenu("TREE", subMenu6);
		glutAddMenuEntry("EXIT", 1);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	/* ======================================= glutMouseFunc ======================================= */

	/* Get mouse position on screen */
	void mouse(int button, int state, int x, int y)
	{
		if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		{
			last = { (float)x, (float)y, 0.0f, 0.0f };
			TheButtonState = GLUT_MIDDLE_BUTTON;
		}
		if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
		{
			TheButtonState = 0;
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			MainMenu();
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			last = { (float)x, (float)y, 0.0f, 1.0f };
			TheButtonState = GLUT_LEFT_BUTTON;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			TheButtonState = 0;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutMotionFunc ======================================= */

	void mouseMotion(int x, int y)
	{
		Vector4Df delta = { last.x - (float)x, (last.y - (float)y)*(-1.0f), 0.0f, 0.0f };
		Matrix4 mToCamera = ToCamera(cam);
		Matrix4 mToWorld = ToWorld(cam);

		if (TheButtonState == GLUT_MIDDLE_BUTTON)
		{
			/* 3D ORBIT: Rotates the view in 3D space */
			if (TheKeyState == GLUT_KEY_CTRL_L)
			{
				Vector4Df eyeToCam = mToCamera * cam.eye; // change to camera
				eyeToCam += delta * (0.2f); // motion
				cam.eye = mToWorld * eyeToCam; // change back to world
			}
			/* PAN: Shifts the view without changing the viewing direction or magnification */
			if (TheKeyState == GLUT_KEY_SHIFT_L)
			{
				// change to camera
				Vector4Df eyeToCam = mToCamera * cam.eye;
				Vector4Df lookatToCam = mToCamera * cam.lookat;
				// motion
				eyeToCam += delta * panSpeed;
				lookatToCam += delta * panSpeed;
				// change back to world
				cam.eye = mToWorld * eyeToCam;
				cam.lookat = mToWorld * lookatToCam;
			}
		}

		if (TheButtonState == GLUT_LEFT_BUTTON)
		{
			/* Translate */
			if (MoveKey == true)
				for (int i = 0; i < objOctree.treeList[index].tree.size(); i++)
				{
					// change to camera
					Vector4Df pointToCam = mToCamera * objOctree.treeList[index].tree[i].point;
					// motion
					pointToCam -= delta * 0.025f;
					// change back to world
					objOctree.treeList[index].tree[i].point = mToWorld * pointToCam;
				}
		}

		last = { (float)x, (float)y, 0.0f, 1.0f };
		glutPostRedisplay();
	}

	/* ======================================= glutMouseWheelFunc ======================================= */

	/* ZOOM: Increases or decreases the magnification of the view in the current viewport */
	void mouseWheel(int button, int dir, int x, int y)
	{
		Vector4Df lookat = cam.lookat - cam.eye;
		//lookat.normalize();
		cam.eye += lookat * (dir*0.1f);
		//cam.lookat += lookat*dir;
		glutPostRedisplay();
	}

	/* ======================================= glutSpecialFunc ======================================= */

	void specialKey(int key, int x, int y)
	{
		switch (key) {
		case GLUT_KEY_F5: // iso view
			cam.eye = { 6.0f, 6.0f, 10.0f };
			cam.lookat = { 0.0f, 0.0f, 0.0f };
			glutPostRedisplay();
			cout << "ISO VIEW" << endl;
			break;
		case GLUT_KEY_F6: // front view
			cam.eye = { 0.0f, 0.0f, 10.0f };
			cam.lookat = { 0.0f, 0.0f, 0.0f };
			glutPostRedisplay();
			cout << "FRONT VIEW" << endl;
			break;
		case GLUT_KEY_F7: // top view
			cam.eye = { 0.0f, 10.0f, 0.1f };
			cam.lookat = { 0.0f, 0.0f, 0.0f };
			glutPostRedisplay();
			cout << "TOP VIEW" << endl;
			break;
		case GLUT_KEY_CTRL_L: // active ROTATE
			TheKeyState = key;
			cout << "3D Orbit ON" << endl;
			break;
		case GLUT_KEY_SHIFT_L: // active PAN
			TheKeyState = key;
			cout << "Pan ON" << endl;
			break;
		case GLUT_KEY_UP: // scale and rotate UP
			if (ScaleKey == true)
			{
				objOctree.treeList[index].tree[0].size *= 1.25f;
				for (int i = 1; i < objOctree.treeList[index].tree.size(); i++)
				{
					objOctree.treeList[index].tree[i].size *= 1.25f;
					Vector4Df vScale = (objOctree.treeList[index].tree[i].point - objOctree.treeList[index].tree[0].point) * 1.25f;
					objOctree.treeList[index].tree[i].point = objOctree.treeList[index].tree[0].point + vScale;
				}
			}
			if (RotateKey == true)
			{
				//Rotation OBJ FILE
				if (inputObjFile.viewEnable == true)
				{
					//ângulo de rotação
					float teta = 15.0f;
					//eixo de rotação
					Vector4Df rotationAxis = { 1.0f, 0.0f, 0.0f, 0.0f };
					inputObjFile.Rotation(inputObjFile.objectList[0].Vertices[0], rotationAxis, teta);
				}

				//Rotation OCTREE
				if (objOctree.viewEnable == true)
				{
					//ângulo de rotação
					float teta = 90.0f;
					//para todos os pontos da octree
					for (int i = 0; i < objOctree.treeList[index].tree.size(); i++)
					{
						Vector4Df position = objOctree.treeList[index].tree[i].point;
						//leva pra origem
						objOctree.treeList[index].tree[i].point = translate(position * (-1.0f)) * objOctree.treeList[index].tree[i].point;
						//faz a rotação
						if (RotateAxis == 'X') objOctree.treeList[index].tree[i].point = rotateX(teta) * objOctree.treeList[index].tree[i].point;
						else if (RotateAxis == 'Y') objOctree.treeList[index].tree[i].point = rotateY(teta) * objOctree.treeList[index].tree[i].point;
						else if (RotateAxis == 'Z') objOctree.treeList[index].tree[i].point = rotateZ(teta) * objOctree.treeList[index].tree[i].point;
						//trás de volta
						objOctree.treeList[index].tree[i].point = translate(position) * objOctree.treeList[index].tree[i].point;
					}
				}
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN: // scale and rotate DOWN
			if (ScaleKey == true)
			{
				objOctree.treeList[index].tree[0].size *= 0.8f;
				for (int i = 1; i < objOctree.treeList[index].tree.size(); i++)
				{
					objOctree.treeList[index].tree[i].size *= 0.8f;
					Vector4Df vScale = (objOctree.treeList[index].tree[i].point - objOctree.treeList[index].tree[0].point) * 0.8f;
					objOctree.treeList[index].tree[i].point = objOctree.treeList[index].tree[0].point + vScale;
				}
			}
			if (RotateKey == true)
			{
				//Rotation OBJ FILE
				if (inputObjFile.viewEnable == true)
				{
					//ângulo de rotação
					float teta = -15.0f;
					//eixo de rotação
					Vector4Df rotationAxis = { 1.0f, 0.0f, 0.0f, 0.0f };

					inputObjFile.Rotation(inputObjFile.objectList[0].Vertices[0], rotationAxis, teta);
				}

				//Rotation OCTREE
				if (objOctree.viewEnable == true)
				{
					//ângulo de rotação
					float teta = -90.0f;
					//para todos os pontos da octree
					for (int i = 0; i < objOctree.treeList[index].tree.size(); i++)
					{
						Vector4Df position = objOctree.treeList[index].tree[i].point;
						//leva pra origem
						objOctree.treeList[index].tree[i].point = translate(position * (-1.0f)) * objOctree.treeList[index].tree[i].point;
						//faz a rotação
						if (RotateAxis == 'X') objOctree.treeList[index].tree[i].point = rotateX(teta) * objOctree.treeList[index].tree[i].point;
						else if (RotateAxis == 'Y') objOctree.treeList[index].tree[i].point = rotateY(teta) * objOctree.treeList[index].tree[i].point;
						else if (RotateAxis == 'Z') objOctree.treeList[index].tree[i].point = rotateZ(teta) * objOctree.treeList[index].tree[i].point;
						//trás de volta
						objOctree.treeList[index].tree[i].point = translate(position) * objOctree.treeList[index].tree[i].point;
					}
				}
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_UP: // size of each voxel
			for (int i = 0; i < objOctree.treeList[index].tree.size(); i++)
			{
				objOctree.treeList[index].tree[i].size *= 1.25f;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_DOWN: // size of each voxel
			for (int i = 0; i < objOctree.treeList[index].tree.size(); i++)
			{
				objOctree.treeList[index].tree[i].size *= 0.8f;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_HOME:
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}

	/* ======================================= glutKeyboardFunc ======================================= */

	void keyboard(unsigned char key, int x, int y)
	{
		switch (key) {
		case 'M':
		case 'm':
			// Move
			MoveKey = true;
			RotateKey = false;
			ScaleKey = false;
			cout << "Move ON" << endl;
			break;
		case 'R':
		case 'r':
			// Rotation Using Quaternions
			MoveKey = false;
			RotateKey = true;
			ScaleKey = false;
			cout << "Rotation Using Quaternions ON" << endl;
			break;
		case 'X':
		case 'x':
			RotateAxis = 'X';
			cout << "Rotate X-axis" << endl;
			break;
		case 'Y':
		case 'y':
			RotateAxis = 'Y';
			cout << "Rotate Y-axis" << endl;
			break;
		case 'Z':
		case 'z':
			RotateAxis = 'Z';
			cout << "Rotate Z-axis" << endl;
			break;
		case 'S': // change select octree
		case 's':
			if (index == objOctree.treeList.size() - 1)
				index = 0;
			else index++;
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= MAIN ======================================= */

	int Start_Window(int argc, char** argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(800, 600);
		glutInitWindowPosition(100, 100);
		glutCreateWindow(argv[0]);
		init();
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMouseFunc(mouse);
		glutMotionFunc(mouseMotion);
		glutMouseWheelFunc(mouseWheel);
		glutSpecialFunc(specialKey);
		glutKeyboardFunc(keyboard);
		glEnable(GL_DEPTH_TEST);
		glutMainLoop();
		return 0;
	}
}
