#pragma once

#include<memory>

#include"SliceView.hpp"
#include"vtkImplicitPlaneWidgetUser.hpp"
#include"vtkCubeSource.h"
#include"vtkTransform.h"

#define WM_CALLBK     WM_USER+1

//#include <msclr\marshal_cppstd.h>  
class vtkImplicitPlaneWidgetUser;

using namespace SliceLib;
//namespace SliceLib
//{
	class SLICELIB_API SliceCouple
	{
	public:
		friend class vtkSliceCoupleCallback;
		SliceCouple(HWND, HWND, HWND, HWND, HWND, HWND);
		~SliceCouple();

	public:
		void PreInit();
		void SetImage(vtkSmartPointer<vtkImageData> imageData);
		void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> iren);
		void Update(int dir, double angle, bool reset = false);
		void TurnOn();
		void TurnOff();
		void UpdateCouple(vtkSmartPointer<vtkTransform>);
		void UpdateCouple(double*);
		void Render();
		double* GetNormal(int);
		double* GetOrigin(int);
		double* GetCenter(int);
		double* GetImageCenter(int);
		void SetDir(int dir);
		void SendMsg();
		void Calibrate();
		void GoCenter();
		void Reset(vtkSmartPointer<vtkImageData> imageData);
		void UpdateSize();

	private:
		std::unique_ptr<double[]> Rotate(double* norm, int dir, double angle);
		void SetSliceView();
		void RotateWithAxes(int, double);

	private:
		SliceView *m_left, *m_right;
		double m_angle;
		int m_dir;
		double* m_bounds;
		HWND m_hwnd;
		HWND m_hnote;

		vtkSmartPointer<vtkImageData> m_imageData;
		vtkSmartPointer<vtkRenderWindowInteractor> m_iren;
		vtkSmartPointer<vtkImplicitPlaneWidgetUser> m_plawi;

		vtkSmartPointer<vtkPolyDataMapper> m_map;
		vtkSmartPointer<vtkActor> m_plane;

		vtkSmartPointer<vtkRenderer> m_renderNote;
		vtkSmartPointer<vtkRenderWindow> m_renWinNote;
		vtkSmartPointer<vtkRenderWindowInteractor> m_irenNote;
	};
//}

///变量是否多余
///显示信息是否多余
///空间是否释放