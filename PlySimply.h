#pragma once
#include <vtkSmartPointer.h>
#include <vtkPLYWriter.h>
#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <cstdio>
#include <iostream>

class PlySimply
{
public:
	PlySimply(const char*sourcePath, const char* destPath, float factor);
	void Compute();
	~PlySimply();
private:
	const char* sourcePath;      //��ǰ��ply�ļ���ַ
	const char* destPath;        //�򻯺��ply�ļ���ַ
	const char* tmpBefore;       //��ʱ�ļ�ǰ��smf��ַ
	const char* tmpAfter;        //��ʱ�ļ򻯺��smf��ַ
	float factor;                //�򻯵�ϵ��
	int verticesNumber;          //��ǰ�Ķ������Ŀ
	int facesNumber;             //��ǰ�����������Ŀ
	int targetFaces;             //�򻯺�����������Ŀ
	void Simply();               //smf�ļ��ļ�
	int Ply2Smf();              //PLY�ļ�ת��Ϊsmf�ļ�
	vtkPolyData *Smf_Reader1();  //�����ǰ��smf�Ķ�ȡ  #$SMF 1.0 ��ͷ
	vtkPolyData *Smf_Reader2();  //����򻯺��smf�Ķ�ȡ  begin��ͷ end��β
	void WritePly();
};

class PLY2BP
{
public:
	PLY2BP(const char* plyPath, const char* bpPath);
	void Write();
private:
	const char* plyPath;
	const char* bpPath;
};