#pragma once

#include<windows.h>

#include"vtkSmartPointer.h"
#include"vtkRenderer.h"
#include"vtkRenderWindow.h"
#include"vtkRenderWindowInteractor.h"
#include"vtkAutoInit.h"
#include"vtkImagePlaneWidget.h"
#include"vtkResliceCursorWidget.h"
#include"vtkResliceCursor.h"
#include"vtkResliceCursorThickLineRepresentation.h"
#include"vtkProperty.h"
#include"vtkImageData.h"
#include"vtkResliceCursorActor.h"
#include"vtkResliceCursorPolyDataAlgorithm.h"
#include"vtkMetaImageReader.h"
#include"vtkCamera.h"
#include"vtkMath.h"
#include"vtkTransform.h"
#include"vtkPlaneSource.h"
#include"vtkPlane.h"
#include"vtkPolyDataMapper.h"
#include"vtkImageViewer2.h"
#include"vtkInteractorStyleTrackballCamera.h"
#include"vtkRendererCollection.h"

#include<array>

#ifdef SLICELIB_EXPORTS
#define SLICELIB_API __declspec(dllexport)
#else
#define SLICELIB_API __declspec(dllimport)
#endif

namespace SliceLib
{
	class SLICELIB_API SliceView
	{
	public:
		SliceView(HWND parent, HWND NOTE);
		~SliceView();
		void PreInit();
		void SetImageData(vtkSmartPointer<vtkImageData>);
		void SetImageData2(vtkSmartPointer<vtkImageData>);
		void SetSliceDirection(int);
		void SetSliceIndex(int);
		void SetCenter(double, double, double);
		vtkSmartPointer<vtkImagePlaneWidget> GetImagePlaneWidget();
		vtkSmartPointer<vtkResliceCursorWidget> GetResliceCursorWidget();
		//vtkSmartPointer<vtkImagePlaneWidget> GetImagePlaneWidget();
		vtkSmartPointer<vtkRenderWindowInteractor> GetInteractor();
		vtkSmartPointer<vtkRenderer> GetRenderer();
		void SetResliceCursor(vtkSmartPointer<vtkResliceCursor>);
		vtkSmartPointer<vtkResliceCursor> GetResliceCursor();
		void Render();
		void RotateTo(double* norm);
		void Update(vtkSmartPointer<vtkTransform>);
		void Update(double*);

		//m_AxialView, m_SagittalView, m_CoronalView, m_3DView
		static void GroupViews(vtkSmartPointer<vtkImageData> imageData,
			std::array<SliceView*, 4> views);

		static void GroupViews(vtkSmartPointer<vtkImageData> imageData,
			SliceView* p_AxialView, SliceView* p_SagittalView,
			SliceView* p_CoronalView, SliceView* p_3DView);
	private:
		void SetupReslice();
		void SetupReslice2();

	private:
		vtkSmartPointer<vtkImageData> m_ImageData;
		vtkSmartPointer<vtkRenderer> m_renderer;
		vtkSmartPointer<vtkRenderWindow>  m_renWin;

		vtkSmartPointer<vtkImagePlaneWidget> m_ImageplaneWidget;
		vtkSmartPointer<vtkResliceCursor> m_ResliceCursor;
		vtkSmartPointer<vtkResliceCursorWidget> m_ResliceCursorWidget;
		vtkSmartPointer<vtkResliceCursorThickLineRepresentation> m_ResliceCursorRep;

		vtkSmartPointer<vtkImageViewer2> m_viewer;


		HWND m_parent;
		HWND m_note;

		int m_direction = 0;
		int m_index = 0;
		double m_center[3];
	};
}


