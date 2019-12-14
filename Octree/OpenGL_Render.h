#pragma once
#include <GL/glut.h>
#include "ObjFile.h"
#include "Octree.h"

namespace OpenGL_Render
{
	void LoadObj(ObjFile &_objFile)
	{
		if (_objFile.viewEnable == false)
			return;
		// DRAW POINTS
		/*glEnable(GL_POINT_SMOOTH);
		glColor3f(0.0f, 0.0f, 0.0f);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < glVertices.size(); i++)
		glVertex3f(glVertices[i].x, glVertices[i].y, glVertices[i].z);
		glEnd();*/

		for (int i = 0; i < _objFile.objectList.size(); i++)
		{
			// DRAW LINES
			glColor3f(0.0f, 0.0f, 0.0f);
			for (int j = 0; j < _objFile.objectList[i].Lines.size(); j++)
			{
				glBegin(GL_LINES);
				glVertex3f((GLfloat)_objFile.objectList[i].Lines[j].vFrom.x, (GLfloat)_objFile.objectList[i].Lines[j].vFrom.y, (GLfloat)_objFile.objectList[i].Lines[j].vFrom.z);
				glVertex3f((GLfloat)_objFile.objectList[i].Lines[j].vTo.x, (GLfloat)_objFile.objectList[i].Lines[j].vTo.y, (GLfloat)_objFile.objectList[i].Lines[j].vTo.z);
				glEnd();
			}

			// DRAW FACES
			//glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for (int j = 0; j < _objFile.objectList[i].Faces.size(); j++)
			{
				glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_POLYGON);
				for (int k = 0; k < _objFile.objectList[i].Faces[j].vertices.size(); k++)
					glVertex3f((GLfloat)_objFile.objectList[i].Faces[j].vertices[k].x, (GLfloat)_objFile.objectList[i].Faces[j].vertices[k].y, (GLfloat)_objFile.objectList[i].Faces[j].vertices[k].z);
				glEnd();
				//arestas
				/*glColor4f(0.8f, 0.3f, 0.8f, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_POLYGON);
				for (int k = 0; k < _objFile.objectList[i].Faces[j].vertices.size(); k++)
					glVertex3f((GLfloat)_objFile.objectList[i].Faces[j].vertices[k].x, (GLfloat)_objFile.objectList[i].Faces[j].vertices[k].y, (GLfloat)_objFile.objectList[i].Faces[j].vertices[k].z);
				glEnd();*/
			}
		}
	}

	void DrawBox(box &oct, bool hit)
	{
		if (oct.status == 'E')
		{
			return;
			/*glColor4f(1.0f, 1.0f, 1.0f, 0.5f);*/
			/*glColor4f(0.5f, 0.5f, 0.5f, 1.0f);*/
			glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (oct.status == 'P')
		{
			return;
			glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (oct.status == 'F')
		{
			/*return;*/
			if (hit == true) glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			else glColor4f(0.0f, 0.7f, 0.0f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
		}
		/* FRONT */
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glEnd();
		/* BACK */
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glEnd();
		/* TOP */
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glEnd();
		/* DOWN */
		glBegin(GL_QUADS);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glEnd();
		/* LEFT */
		glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glEnd();
		/* RIGHT */
		glBegin(GL_QUADS);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y + oct.size, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z + oct.size);
		glVertex3f((GLfloat)oct.point.x + oct.size, (GLfloat)oct.point.y, (GLfloat)oct.point.z);
		glEnd();
	}

	void LoadOct(Octree &_objOctree, int index)
	{
		if (_objOctree.viewEnable == false)
			return;

		for (int i = 0; i < _objOctree.treeList.size(); i++)
		{
			bool selected = false;
			if (i == index) selected = true;
			for (int j = 0; j < _objOctree.treeList[i].tree.size(); j++)
				DrawBox(_objOctree.treeList[i].tree[j], selected);
		}
			
				
	}
}
