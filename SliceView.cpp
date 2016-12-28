
#include "SliceView.hpp"
//#include"vtkResliceCursorCallback.hpp"

VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

namespace SliceLib
{
	SliceView::SliceView(HWND parent, HWND NOTE)
	{
		m_parent = parent;
		m_note = NOTE;
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

		//m_ResliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
		//m_ResliceCursorWidget = vtkSmartPointer<vtkResliceCursorWidget>::New();
		//m_ResliceCursorRep = vtkSmartPointer<vtkResliceCursorThickLineRepresentation>::New();
		//m_ResliceCursorWidget->SetRepresentation(m_ResliceCursorRep);
		//m_ResliceCursorWidget->SetDefaultRenderer(m_renderer);
		//m_ResliceCursorRep->ShowReslicedImageOff();

		std::unique_ptr<RECT> prect(new RECT());
		GetWindowRect(m_parent, prect.get());

		//m_viewer = vtkSmartPointer<vtkImageViewer2>::New();
		//m_viewer->SetParentId(m_parent);
		//m_viewer->GetRenderWindow()->SetSize(prect->right - prect->left,
		//	prect->bottom - prect->top);

		m_tool = std::unique_ptr<VolumeRenderingToolSlice>(new VolumeRenderingToolSlice());
		m_renderertool = vtkSmartPointer<vtkRenderer>::New();
		m_renWintool = vtkSmartPointer<vtkRenderWindow>::New();
		m_renWintool->SetSize(prect->right - prect->left, prect->bottom - prect->top);
		m_renWintool->SetParentId(m_parent);
		m_renWintool->AddRenderer(m_renderertool);
		if (m_renWintool->GetInteractor() == nullptr)
		{
			auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
			iren->SetRenderWindow(m_renWintool);
			iren->Initialize();
		}
		

		GetWindowRect(m_note, prect.get());
		m_renderer = vtkSmartPointer<vtkRenderer>::New();
		m_renWin = vtkSmartPointer<vtkRenderWindow>::New();
		m_renWin->SetSize(prect->right - prect->left, prect->bottom - prect->top);
		m_renWin->SetParentId(m_note);		
		m_renWin->AddRenderer(m_renderer);

		if (m_renWin->GetInteractor() == nullptr)
		{
			auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
			iren->SetRenderWindow(m_renWin);
			//m_ResliceCursorWidget->SetInteractor(iren);
			//m_ImageplaneWidget->SetInteractor(iren);
			iren->Initialize();
		}
		//m_renWin->Start();
	}

	void SliceView::SetSliceDirection(int direction)
	{
		m_direction = direction;
	}

	void SliceView::SetSliceIndex(int index)
	{
		m_index = index;
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
		//auto camera = m_renderer->GetActiveCamera();
		//
		//double* vpn = camera->GetViewPlaneNormal();
		//double* norm = m_ImageplaneWidget->GetNormal();
		//double cosin = vtkMath::Dot(vpn, norm) / (vtkMath::Norm(vpn)*vtkMath::Norm(norm));
		//std::cout << "angle: " << cosin << std::endl;

		//double* pos = camera->GetPosition();
		//double* foc = camera->GetFocalPoint();
		//double diff[3];
		//diff[0] = pos[0] - foc[0];
		//diff[1] = pos[1] - foc[1];
		//diff[2] = pos[2] - foc[2];
		//double dis = vtkMath::Norm(diff);
		//std::cout << "distance: " << dis << std::endl;
		//
		//camera->SetFocalPoint(m_ImageplaneWidget->GetCenter());
		
		//camera->SetPosition()
		//m_renderer->ResetCamera();
		//m_renderer->GetActiveCamera()->
		m_renWin->Render();
		m_renWintool->Render();
		//m_viewer->Render();
	}

	void SliceView::SetCenter(double c0, double c1, double c2)
	{
		m_center[0] = c0;
		m_center[1] = c1;
		m_center[2] = c2;
	}

	void SliceView::ResetImageData2(vtkSmartPointer<vtkImageData> imageData)
	{
		if (nullptr == imageData)
		{
			return;
		}
		m_ImageData = imageData;

		m_ImageplaneWidget->SetInputData(m_ImageData);
		m_ImageplaneWidget->SetPlaneOrientation(m_direction);
		m_ImageplaneWidget->SetSliceIndex(m_index);

		m_tool->SetImageData(m_ImageplaneWidget->GetResliceOutput());
		m_tool->UpdateImage();
		m_renderertool->ResetCamera();
		m_renWintool->Render();

		//double* range = m_ImageData->GetScalarRange();
		//m_viewer->SetInputData(m_ImageplaneWidget->GetResliceOutput());
		//m_viewer->SetColorLevel((range[1] - range[0]) / 2);
		//m_viewer->SetColorWindow(range[1] - range[0]);
		//m_viewer->SetSlice(1);
		//m_viewer->SetSliceOrientationToXY();
		//m_viewer->GetImageActor()->RotateX(180);
		//m_viewer->Render();
		//m_ImageplaneWidget->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
	}

	void SliceView::SetImageData2(vtkSmartPointer<vtkImageData> imageData)
	{
		if (nullptr == imageData)
		{
			return;
		}
		m_ImageData = imageData;
		SetupReslice2();
	}

	void SliceView::SetupReslice2()
	{
		if (nullptr == m_ImageData)
		{
			return;
		}
		//m_ResliceCursor->SetCenter(m_center);
		//m_ResliceCursor->SetImage(m_ImageData);
		//m_ResliceCursor->SetThickMode(0);

		//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);

		////m_ResliceCursorRep->GetPlaneSource()->SetCenter(m_center);
		////m_ResliceCursorRep->GetPlaneSource()->SetNormal(0, 1, 0);
		////m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->
		//
		//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(m_direction);
		//if (m_direction==1)
		//	m_ResliceCursor->GetPlane(m_direction)->SetNormal(0, 0, 1);

		////m_ResliceCursorWidget->

		////m_renderer->GetActiveCamera()->OrthogonalizeViewUp();
		////if (m_direction == 1)
		////{
		////	m_renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
		////	//m_renderer->GetActiveCamera()->Elevation(90);
		////}
		////m_ResliceCursorWidget->SetEnabled(1);
		//m_ResliceCursorWidget->On();
		//m_renderer->ResetCamera();

		//double* range = m_ImageData->GetScalarRange();
		////m_ImageplaneWidget->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
		//m_ResliceCursorWidget->GetResliceCursorRepresentation()
		//	->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);


		m_ImageplaneWidget->SetInputData(m_ImageData);
		m_ImageplaneWidget->SetPlaneOrientation(m_direction);

		//int* dims = m_ImageData->GetDimensions();
		//if (dims[m_direction] < m_index) m_index = dims[m_direction];
		m_ImageplaneWidget->SetSliceIndex(m_index);
		m_ImageplaneWidget->On();
		//m_ImageplaneWidget->InteractionOn();

		////////////using VolumeRenderingToolSlice
		//auto imageData = vtkSmartPointer<vtkImageData>::New();
		//auto info = imageData->GetInformation();
		//imageData->SetDimensions(60, 98, 1);
		//imageData->SetSpacing(0.28, 0.28, 0.04);
		//imageData->SetScalarType(VTK_UNSIGNED_SHORT, info);
		//imageData->SetNumberOfScalarComponents(1, info);
		//imageData->AllocateScalars(info);
		//unsigned short* ptr = (unsigned short*)(imageData->GetScalarPointer());
		//for (int i = 0; i < 60 * 98; i++)
		//{
		//	*(ptr++) = 10;
		//}
		//m_tool->SetImageData(imageData);
		m_tool->SetImageData(m_ImageplaneWidget->GetResliceOutput());	
		m_tool->Update2();
		m_renderertool->AddActor(m_tool->GetVolume());
		m_renderertool->ResetCamera();
		m_renWintool->Render();

		////using vtkImageViewer2
		//double* range = m_ImageData->GetScalarRange();
		//m_viewer->SetInputData(m_ImageplaneWidget->GetResliceOutput());
		//m_viewer->SetColorLevel((range[1] - range[0]) / 2);
		//m_viewer->SetColorWindow(range[1] - range[0]);
		//m_viewer->SetSlice(1);
		//m_viewer->SetSliceOrientationToXY();
		//m_viewer->GetImageActor()->RotateX(180);
		//m_viewer->Render();
		//m_ImageplaneWidget->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
	}

	void SliceView::RotateTo(double* norm)
	{
		//GetImagePlaneWidget()->SetNormal
		double here[3];
		GetImagePlaneWidget()->GetNormal(here);
		double axis[3];
		double angle = vtkMath::AngleBetweenVectors(here, norm) / vtkMath::Pi() * 180;
		vtkMath::Cross(here, norm, axis);
		auto transform = vtkSmartPointer<vtkTransform>::New();
		transform->RotateWXYZ(angle, axis);

		// Modify and update planeWidget  
		double newpt[3];
		transform->TransformPoint(GetImagePlaneWidget()->GetPoint1(), newpt);
		GetImagePlaneWidget()->SetPoint1(newpt);
		transform->TransformPoint(GetImagePlaneWidget()->GetPoint2(), newpt);
		GetImagePlaneWidget()->SetPoint2(newpt);
		transform->TransformPoint(GetImagePlaneWidget()->GetOrigin(), newpt);
		GetImagePlaneWidget()->SetOrigin(newpt);
		GetImagePlaneWidget()->UpdatePlacement();
	}

	void SliceView::Update(vtkSmartPointer<vtkTransform> transform)
	{
		double newpt[3];
		transform->TransformPoint(m_ImageplaneWidget->GetPoint1(), newpt);
		m_ImageplaneWidget->SetPoint1(newpt);
		transform->TransformPoint(m_ImageplaneWidget->GetPoint2(), newpt);
		m_ImageplaneWidget->SetPoint2(newpt);
		transform->TransformPoint(m_ImageplaneWidget->GetOrigin(), newpt);
		m_ImageplaneWidget->SetOrigin(newpt);
		m_ImageplaneWidget->UpdatePlacement();
	}

	void SliceView::Update(double* bds)
	{
		auto ps = static_cast<vtkPlaneSource*>(m_ImageplaneWidget->GetPolyDataAlgorithm());
		ps->SetPoint1(bds[0], bds[2], bds[4]);
		ps->SetPoint2(bds[1], bds[3], bds[5]);
		ps->SetOrigin(bds[0], bds[3], bds[5]);
	}


	///////////////not implemented
	void SliceView::SetImageData(vtkSmartPointer<vtkImageData> imageData)
	{
		//if (nullptr == imageData)
		//{
		//	return;
		//}
		//m_ResliceCursor = vtkSmartPointer<vtkResliceCursor>::New();
		//m_ResliceCursorWidget = vtkSmartPointer<vtkResliceCursorWidget>::New();
		//m_ResliceCursorRep = vtkSmartPointer<vtkResliceCursorThickLineRepresentation>::New();
		//m_ResliceCursorWidget->SetRepresentation(m_ResliceCursorRep);
		//m_ResliceCursorWidget->SetDefaultRenderer(m_renderer);

		//m_ImageData = imageData;
		//SetupReslice();
	}

	void SliceView::SetupReslice()
	{
		//if (nullptr == m_ImageData)
		//{
		//	return;
		//}
		//int* dims = m_ImageData->GetDimensions();

		//m_ImageplaneWidget->SetInputData(m_ImageData);
		//m_ImageplaneWidget->SetPlaneOrientation(m_direction);
		//m_ImageplaneWidget->SetSliceIndex(dims[m_direction] / 2);
		//m_ImageplaneWidget->On();
		//m_ImageplaneWidget->InteractionOn();

		//m_ResliceCursor->SetCenter(m_ImageData->GetCenter());
		//m_ResliceCursor->SetImage(m_ImageData);
		//m_ResliceCursor->SetThickMode(0);

		//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);
		//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(m_direction);

		////m_renderer->GetActiveCamera()->OrthogonalizeViewUp();
		//
		////m_renderer->GetActiveCamera()->Roll(90);
		//if (m_direction == 1)
		//{
		//	m_renderer->GetActiveCamera()->SetViewUp(0, 0, 1);

		//	//m_renderer->GetActiveCamera()->Elevation(90);
		//}
		////m_ResliceCursorWidget->SetEnabled(1);
		//m_ResliceCursorWidget->On();
		//m_renderer->ResetCamera();

		//double* range = m_ImageData->GetScalarRange();
		//m_ImageplaneWidget->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
		//m_ResliceCursorWidget->GetResliceCursorRepresentation()
		//	->SetWindowLevel(range[1] - range[0], (range[1] - range[0]) / 2);
	}

	//m_AxialView, m_SagittalView, m_CoronalView, m_3DView
	void SliceView::GroupViews(vtkSmartPointer<vtkImageData> imageData,
		SliceView* p_AxialView, SliceView* p_SagittalView,
		SliceView* p_CoronalView, SliceView* p_3DView)
	{
		//if (nullptr == imageData) return;

		//p_AxialView->SetSliceDirection(0);
		//p_AxialView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());//(m_3DView.GetInteractor());//?
		////m_AxialView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());//?
		//p_AxialView->GetResliceCursorWidget()->SetInteractor(p_AxialView->GetInteractor());

		//p_SagittalView->SetSliceDirection(1);
		//p_SagittalView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());
		////m_SagittalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		//p_SagittalView->SetResliceCursor(p_AxialView->GetResliceCursor());
		//p_SagittalView->GetResliceCursorWidget()->SetInteractor(p_SagittalView->GetInteractor());

		//p_CoronalView->SetSliceDirection(2);
		//p_CoronalView->GetImagePlaneWidget()->SetInteractor(p_3DView->GetInteractor());
		////m_CoronalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		//p_CoronalView->SetResliceCursor(p_AxialView->GetResliceCursor());
		//p_CoronalView->GetResliceCursorWidget()->SetInteractor(p_CoronalView->GetInteractor());

		//auto cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		//cbk->view[0] = p_AxialView;
		//p_AxialView->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[1] = p_SagittalView;
		//p_SagittalView->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[2] = p_CoronalView;
		//p_CoronalView->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[3] = p_3DView;

		//p_AxialView->SetImageData(imageData);
		//p_SagittalView->SetImageData(imageData);
		//p_CoronalView->SetImageData(imageData);

		//p_AxialView->Render();
		//p_SagittalView->Render();
		//p_CoronalView->Render();
		//p_3DView->GetRenderer()->ResetCamera();
		//p_3DView->Render();
	}

	void SliceView::GroupViews(vtkSmartPointer<vtkImageData> imageData,
		std::array<SliceView*, 4> views)
	{
		//if (nullptr == imageData) return;

		//views[0]->SetSliceDirection(0);
		//views[0]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());//(m_3DView.GetInteractor());//?
		////m_AxialView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());//?
		//views[0]->GetResliceCursorWidget()->SetInteractor(views[0]->GetInteractor());

		//views[1]->SetSliceDirection(1);
		//views[1]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());
		////m_SagittalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		//views[1]->SetResliceCursor(views[0]->GetResliceCursor());
		//views[1]->GetResliceCursorWidget()->SetInteractor(views[1]->GetInteractor());

		//views[2]->SetSliceDirection(2);
		//views[2]->GetImagePlaneWidget()->SetInteractor(views[3]->GetInteractor());
		////m_CoronalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());
		//views[2]->SetResliceCursor(views[0]->GetResliceCursor());
		//views[2]->GetResliceCursorWidget()->SetInteractor(views[2]->GetInteractor());

		//auto cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
		//cbk->view[0] = views[0];
		//views[0]->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[1] = views[1];
		//views[1]->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[2] = views[2];
		//views[2]->GetResliceCursorWidget()->AddObserver(
		//	vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);

		//cbk->view[3] = views[3];

		//for (int i = 0; i < 3; i++)
		//{
		//	views[i]->SetImageData(imageData);
		//	views[i]->Render();
		//}
		//views[3]->GetRenderer()->ResetCamera();
		//views[3]->Render();
	}
}

