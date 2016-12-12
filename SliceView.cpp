
#include "SliceView.hpp"
#include"vtkResliceCursorCallback.hpp"
#include<memory>

VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

namespace SliceLib
{
	SliceView::SliceView(HWND parent)
	{
		m_parent = parent;
	}


	SliceView::~SliceView()
	{
	}


	void SliceView::PreInit()
	{
		// TODO: Add your specialized code here and/or call the base class
		if (!m_parent)
		{
			return;
		}
		m_ImageplaneWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
		m_ImageplaneWidget->GetPlaneProperty()->SetColor(0, 0, 0);
		m_ImageplaneWidget->SetResliceInterpolateToLinear();
		m_ImageplaneWidget->RestrictPlaneToVolumeOn();
		m_ImageplaneWidget->DisplayTextOn();

		m_ResliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
		m_ResliceCursorWidget = vtkSmartPointer<vtkResliceCursorWidget>::New();
		m_ResliceCursorRep = vtkSmartPointer<vtkResliceCursorThickLineRepresentation>::New();
		m_ResliceCursorWidget->SetRepresentation(m_ResliceCursorRep);
		m_ResliceCursorWidget->SetDefaultRenderer(m_renderer);

		std::unique_ptr<RECT> prect(new RECT());
		GetWindowRect(m_parent, prect.get());
		m_renderer = vtkSmartPointer<vtkRenderer>::New();
		m_renWin = vtkSmartPointer<vtkRenderWindow>::New();
		m_renWin->SetParentId(this->m_parent);
		m_renWin->SetSize(prect->right - prect->left, prect->bottom - prect->top);
		m_renWin->AddRenderer(m_renderer);

		if (m_renWin->GetInteractor() == nullptr)
		{
			auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
			iren->SetRenderWindow(m_renWin);
			//m_ResliceCursorWidget->SetInteractor(iren);
			//m_ImageplaneWidget->SetInteractor(iren);
			iren->Initialize();
		}
		m_renWin->Start();
	}

	void SliceView::SetImageData(vtkSmartPointer<vtkImageData> imageData)
	{
		if (nullptr == imageData)
		{
			return;
		}
		m_ImageData = imageData;
		SetupReslice();
	}

	void SliceView::SetupReslice()
	{
		if (nullptr == m_ImageData)
		{
			return;
		}
		int* dims = m_ImageData->GetDimensions();

		m_ImageplaneWidget->SetInputData(m_ImageData);
		m_ImageplaneWidget->SetPlaneOrientation(m_direction);
		m_ImageplaneWidget->SetSliceIndex(dims[m_direction] / 2);
		m_ImageplaneWidget->On();
		m_ImageplaneWidget->InteractionOn();

		m_ResliceCursor->SetCenter(m_ImageData->GetCenter());
		m_ResliceCursor->SetImage(m_ImageData);
		m_ResliceCursor->SetThickMode(0);

		m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);
		m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(m_direction);

		//m_renderer->GetActiveCamera()->OrthogonalizeViewUp();
		
		//m_renderer->GetActiveCamera()->Roll(90);
		if (m_direction == 1)
		{
			m_renderer->GetActiveCamera()->SetViewUp(0, 0, 1);

			//m_renderer->GetActiveCamera()->Elevation(90);
		}
		//m_ResliceCursorWidget->SetEnabled(1);
		m_ResliceCursorWidget->On();
		m_renderer->ResetCamera();

		double* range = m_ImageData->GetScalarRange();
		m_ImageplaneWidget->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
		m_ResliceCursorWidget->GetResliceCursorRepresentation()
			->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
	}

	void SliceView::SetSliceDirection(int direction)
	{
		m_direction = direction;
	}

	vtkSmartPointer<vtkImagePlaneWidget> SliceView::GetImagePlaneWidget()
	{
		return m_ImageplaneWidget;
	}

	vtkSmartPointer<vtkResliceCursorWidget> SliceView::GetResliceCursorWidget()
	{
		return m_ResliceCursorWidget;
	}

	vtkSmartPointer<vtkRenderWindowInteractor> SliceView::GetInteractor()
	{
		return m_renWin->GetInteractor();
	}

	vtkSmartPointer<vtkRenderer> SliceView::GetRenderer()
	{
		return m_renderer;
	}

	void SliceView::SetResliceCursor(vtkSmartPointer<vtkResliceCursor> cursor)
	{
		m_ResliceCursor = cursor;
	}

	vtkSmartPointer<vtkResliceCursor> SliceView::GetResliceCursor()
	{
		return m_ResliceCursor;
	}

	void SliceView::Render()
	{
		m_renWin->Render();
	}

	//m_AxialView, m_SagittalView, m_CoronalView, m_3DView
	void SliceView::GroupViews(vtkSmartPointer<vtkImageData> imageData,
		SliceView* p_AxialView, SliceView* p_SagittalView,
		SliceView* p_CoronalView, SliceView* p_3DView)
	{
		if (nullptr == imageData) return;

		p_AxialView->SetSliceDirection(0);
		p_AxialView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());//(m_3DView.GetInteractor());//?
		//m_AxialView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());//?
		p_AxialView->GetResliceCursorWidget()->SetInteractor(p_AxialView->GetInteractor());

		p_SagittalView->SetSliceDirection(1);
		p_SagittalView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());
		//m_SagittalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		p_SagittalView->SetResliceCursor(p_AxialView->GetResliceCursor());
		p_SagittalView->GetResliceCursorWidget()->SetInteractor(p_SagittalView->GetInteractor());

		p_CoronalView->SetSliceDirection(2);
		p_CoronalView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());
		//m_CoronalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		p_CoronalView->SetResliceCursor(p_AxialView->GetResliceCursor());
		p_CoronalView->GetResliceCursorWidget()->SetInteractor(p_CoronalView->GetInteractor());

		auto cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		cbk->view[0] = p_AxialView;
		p_AxialView->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[1] = p_SagittalView;
		p_SagittalView->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[2] = p_CoronalView;
		p_CoronalView->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[3] = p_3DView;

		p_AxialView->SetImageData(imageData);
		p_SagittalView->SetImageData(imageData);
		p_CoronalView->SetImageData(imageData);

		p_AxialView->Render();
		p_SagittalView->Render();
		p_CoronalView->Render();
		p_3DView->GetRenderer()->ResetCamera();
		p_3DView->Render();
	}

	void SliceView::GroupViews(vtkSmartPointer<vtkImageData> imageData,
		std::array<SliceView*, 4> views)
	{
		if (nullptr == imageData) return;

		views[0]->SetSliceDirection(0);
		views[0]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());//(m_3DView.GetInteractor());//?
		//m_AxialView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());//?
		views[0]->GetResliceCursorWidget()->SetInteractor(views[0]->GetInteractor());

		views[1]->SetSliceDirection(1);
		views[1]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());
		//m_SagittalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		views[1]->SetResliceCursor(views[0]->GetResliceCursor());
		views[1]->GetResliceCursorWidget()->SetInteractor(views[1]->GetInteractor());

		views[2]->SetSliceDirection(2);
		views[2]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());
		//m_CoronalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		views[2]->SetResliceCursor(views[0]->GetResliceCursor());
		views[2]->GetResliceCursorWidget()->SetInteractor(views[2]->GetInteractor());

		auto cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		cbk->view[0] = views[0];
		views[0]->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[1] = views[1];
		views[1]->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[2] = views[2];
		views[2]->GetResliceCursorWidget()->AddObserver(
			vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		cbk->view[3] = views[3];

		for (int i = 0; i < 3; i++)
		{
			views[i]->SetImageData(imageData);
		}
	}
}

