#include"SliceCouple.hpp"

//namespace SliceLib
//{
	SliceCouple::SliceCouple(HWND LEFT, HWND RIGHT, HWND NOTE, HWND NOTEL, HWND NOTER, HWND FORM)
	{
		m_left = new SliceView(LEFT, NOTEL);
		m_right = new SliceView(RIGHT, NOTER);
		m_bounds = nullptr;
		m_hwnd = FORM;
		m_hnote = NOTE;
		m_dir = 0;
		m_angle = 0;
	}

	SliceCouple::~SliceCouple()
	{
		delete m_left;
		delete m_right;
	}

	void SliceCouple::UpdateSize()
	{
		std::unique_ptr<RECT> prect(new RECT());
		GetWindowRect(m_hnote, prect.get());
		m_renWinNote->SetSize(prect->right - prect->left,
			prect->bottom - prect->top);
		m_renWinNote->Render();
		m_left->UpdateSize();
		m_right->UpdateSize();
	}

	void SliceCouple::PreInit()
	{
		m_left->PreInit();
		m_right->PreInit();
	}

	void SliceCouple::SetImage(vtkSmartPointer<vtkImageData> imageData)
	{
		m_imageData = imageData;
		m_bounds = imageData->GetBounds();
		//std::cout << "slicebds: " << m_bounds[0] << "  " << m_bounds[1] << "  "
		//	<< m_bounds[2] << "  " << m_bounds[3] << "  "
		//	<< m_bounds[4] << "  " << m_bounds[5] << "  " << std::endl;
	}

	void SliceCouple::SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> iren)
	{
		m_iren = iren;
	}

	void SliceCouple::SetDir(int dir)
	{
		m_dir = dir;
	}

	void SliceCouple::Update(int dir, double angle, bool reset)
	{
		if (dir == 0) return;
		else if (reset)
		{
			switch (dir)
			{
			case 1://axes y
			{
				m_plawi->SetNormal(0, 1, 0);
				m_left->GetImagePlaneWidget()->SetPlaneOrientationToYAxes();
				m_right->GetImagePlaneWidget()->SetPlaneOrientationToYAxes();
				double* origin = m_plawi->GetOrigin();
				int* dims = m_imageData->GetDimensions();
				int ind = (origin[1] - m_bounds[2]) / (m_bounds[3] - m_bounds[2])*dims[1];
				m_left->GetImagePlaneWidget()->SetSliceIndex(ind);
				m_right->GetImagePlaneWidget()->SetSliceIndex(ind + 1);
				break;
			}			
			case 2://axes z
			{
				m_plawi->SetNormal(0, 0, 1);
				m_left->GetImagePlaneWidget()->SetPlaneOrientationToZAxes();
				m_right->GetImagePlaneWidget()->SetPlaneOrientationToZAxes();
				double* origin = m_plawi->GetOrigin();
				int* dims = m_imageData->GetDimensions();
				int ind = (origin[2] - m_bounds[4]) / (m_bounds[5] - m_bounds[4])*dims[2];
				m_left->GetImagePlaneWidget()->SetSliceIndex(ind);
				m_right->GetImagePlaneWidget()->SetSliceIndex(ind + 1);
				break;
			}			
			default://axes x
				m_plawi->SetNormal(1,0,0);
				m_left->GetImagePlaneWidget()->SetPlaneOrientationToXAxes();
				m_right->GetImagePlaneWidget()->SetPlaneOrientationToXAxes();
				double* origin = m_plawi->GetOrigin();
				int* dims = m_imageData->GetDimensions();
				int ind = (origin[0] - m_bounds[0]) / (m_bounds[1] - m_bounds[0])*dims[0];
				m_left->GetImagePlaneWidget()->SetSliceIndex(ind);
				m_right->GetImagePlaneWidget()->SetSliceIndex(ind + 1);
				break;
			}
			m_dir = dir;
			m_angle = angle;
		}
		else
		{
			if (m_dir == dir)
			{
				double r_angle = angle - m_angle;
				m_angle = angle;

				RotateWithAxes(dir, r_angle);

				//double* norm = m_plawi->GetNormal();
				//auto pos_norm = Rotate(norm, dir, r_angle);
				//m_plawi->SetNormal(pos_norm.get());
			}
		}
		Render();
		//PostMessage(m_hwnd, WM_CALLBK, 0, 0);
	}

	void SliceCouple::Calibrate()
	{
		m_plawi->Calibrate();
	}

	void SliceCouple::GoCenter()
	{
		m_plawi->GoCenter();
	}

	void SliceCouple::SendMsg()
	{
		PostMessage(m_hwnd, WM_CALLBK, 0, 0);
	}

	void SliceCouple::RotateWithAxes(int dir, double angle)
	{
		double* origin = this->m_plawi->GetOrigin();//旋转中心点

		double* norm = m_plawi->GetNormal();
		auto pos_norm = Rotate(norm, dir, angle);
		m_plawi->SetNormal(pos_norm.get());

		double* p = m_left->GetImagePlaneWidget()->GetPoint1();
		double dp[3];
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		double* pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_left->GetImagePlaneWidget()->SetPoint1(pdp);

		p = m_left->GetImagePlaneWidget()->GetPoint2();
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_left->GetImagePlaneWidget()->SetPoint2(pdp);

		p = m_left->GetImagePlaneWidget()->GetOrigin();
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_left->GetImagePlaneWidget()->SetOrigin(pdp);

		p = m_right->GetImagePlaneWidget()->GetPoint1();
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_right->GetImagePlaneWidget()->SetPoint1(pdp);

		p = m_right->GetImagePlaneWidget()->GetPoint2();
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_right->GetImagePlaneWidget()->SetPoint2(pdp);

		p = m_right->GetImagePlaneWidget()->GetOrigin();
		dp[0] = p[0] - origin[0]; dp[1] = p[1] - origin[1]; dp[2] = p[2] - origin[2];
		pos_norm = Rotate(dp, dir, angle);
		pdp = pos_norm.get();
		pdp[0] = pdp[0] + origin[0]; pdp[1] = pdp[1] + origin[1]; pdp[2] = pdp[2] + origin[2];
		m_right->GetImagePlaneWidget()->SetOrigin(pdp);

		m_left->GetImagePlaneWidget()->UpdatePlacement();
		m_right->GetImagePlaneWidget()->UpdatePlacement();

		//switch (dir)
		//{
		//case 1://normal 平行于 Y，绕X轴旋转
		//{
		//	double* origin = this->m_plawi->GetOrigin();
		//	double oNew[3];
		//	auto transform = vtkSmartPointer<vtkTransform>::New();
		//	transform->Identity();
		//	transform->Translate(origin[0], origin[1], origin[2]);
		//	transform->RotateX(angle);
		//	transform->Translate(-origin[0], -origin[1], -origin[2]);
		//	transform->TransformNormal(this->m_plawi->GetNormal(), oNew);
		//	this->m_plawi->SetNormal(oNew);
		//	this->UpdateCouple(transform);
		//}
		//case 2://normal 平行于 Z，绕Y轴旋转
		//{
		//	double* origin = this->m_plawi->GetOrigin();
		//	double oNew[3];
		//	auto transform = vtkSmartPointer<vtkTransform>::New();
		//	transform->Identity();
		//	transform->Translate(origin[0], origin[1], origin[2]);
		//	transform->RotateY(angle);
		//	transform->Translate(-origin[0], -origin[1], -origin[2]);
		//	transform->TransformNormal(this->m_plawi->GetNormal(), oNew);
		//	this->m_plawi->SetNormal(oNew);
		//	this->UpdateCouple(transform);
		//}
		//default://normal 平行于 X，绕Z轴旋转
		//{
		//	double* origin = this->m_plawi->GetOrigin();
		//	double oNew[3];
		//	auto transform = vtkSmartPointer<vtkTransform>::New();
		//	transform->Identity();
		//	transform->Translate(origin[0], origin[1], origin[2]);
		//	transform->RotateZ(angle);
		//	transform->Translate(-origin[0], -origin[1], -origin[2]);
		//	transform->TransformNormal(this->m_plawi->GetNormal(), oNew);
		//	this->m_plawi->SetNormal(oNew);
		//	this->UpdateCouple(transform);
		//}
		//}
	}

	void SliceCouple::UpdateCouple(vtkSmartPointer<vtkTransform> transform)
	{
		m_left->Update(transform);
		m_right->Update(transform);
	}
	void SliceCouple::UpdateCouple(double* bds)
	{
		m_left->Update(bds);
		m_right->Update(bds);
	}


	void SliceCouple::Render()
	{
		m_left->Render();
		m_right->Render();
		m_iren->Render();
		m_irenNote->Render();
		//vtkImageData* image = m_left->GetImagePlaneWidget()->GetResliceOutput();
		//int* dims = image->GetDimensions();
		//double* b = image->GetBounds();
		//std::cout << "dims: " << dims[0] << "  " << dims[1] << "  " << dims[2] << std::endl;
	}

	double* SliceCouple::GetNormal(int flag)
	{
		if (0 == flag)
		{
			auto ps = static_cast<vtkPlaneSource*>(m_left->GetImagePlaneWidget()
				->GetPolyDataAlgorithm());
			return ps->GetNormal();
			//return m_left->GetImagePlaneWidget()->GetNormal();
		}
		else
		{
			auto ps = static_cast<vtkPlaneSource*>(m_right->GetImagePlaneWidget()
				->GetPolyDataAlgorithm());
			return ps->GetNormal();
			//return m_right->GetImagePlaneWidget()->GetNormal();
		}
	}

	double* SliceCouple::GetOrigin(int flag)
	{
		if (0 == flag)
		{
			return m_left->GetImagePlaneWidget()->GetOrigin();
		}
		else
		{
			return m_right->GetImagePlaneWidget()->GetOrigin();
		}
	}

	double* SliceCouple::GetCenter(int flag)
	{
		if (0 == flag)
		{
			return m_left->GetImagePlaneWidget()->GetCenter();
		}
		return m_right->GetImagePlaneWidget()->GetCenter();
		
	}

	double* SliceCouple::GetImageCenter(int flag)
	{
		if (0 == flag)
		{
			return m_left->GetImagePlaneWidget()->GetResliceOutput()->GetCenter();
		}
		return m_right->GetImagePlaneWidget()->GetResliceOutput()->GetCenter();
	}

	void SliceCouple::TurnOff()
	{
		m_plawi->GetCurrentRenderer()->RemoveActor(m_plane);
		m_iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(m_plane);
		m_plawi->EnabledOff();
		m_iren->Render();
	}

	void SliceCouple::Reset(vtkSmartPointer<vtkImageData> imageData)
	{
		m_imageData = imageData;
		m_bounds = imageData->GetBounds();
		m_plawi->PlaceWidget(m_imageData->GetBounds());
		m_plawi->SetOrigin(m_imageData->GetCenter());

		//////left and right
		int* dims = m_imageData->GetDimensions();
		m_left->SetSliceDirection(0);
		m_left->SetSliceIndex(dims[0] / 2);

		m_right->SetSliceDirection(0);
		m_right->SetSliceIndex(dims[0] / 2 + 1);

		m_left->ResetImageData2(m_imageData);
		m_right->ResetImageData2(m_imageData);

		//auto ps = static_cast<vtkPlaneSource*>(m_left->GetImagePlaneWidget()
		//	->GetPolyDataAlgorithm());

		//m_map->RemoveAllInputs();
		//m_map->SetInputConnection(ps->GetOutputPort());

		this->Render();
	}

	void SliceCouple::TurnOn()
	{
		//First, SliceView

		//////////////////////////////////////////////////////////////////////////

		//Second, SlicePlaneView
		//double* bounds = m_bounds;
		//double* bounds = m_imageData->GetBounds();
		m_renderNote = vtkSmartPointer<vtkRenderer>::New();
		m_renWinNote = vtkSmartPointer<vtkRenderWindow>::New();
		m_renWinNote->AddRenderer(m_renderNote);

		std::unique_ptr<RECT> prect(new RECT());
		GetWindowRect(m_hnote, prect.get());
		m_renWinNote->SetSize(prect->right - prect->left,
			prect->bottom - prect->top);

		m_renWinNote->SetParentId(m_hnote);
		m_irenNote = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		m_renWinNote->SetInteractor(m_irenNote);
		auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		m_irenNote->SetInteractorStyle(style);
		m_irenNote->Initialize();

		m_plawi = vtkSmartPointer<vtkImplicitPlaneWidgetUser>::New();
		m_plawi->SetCouple(this);
		m_plawi->SetInteractor(m_irenNote);
		//m_plawi->SetInputData(m_imageData);
		m_plawi->SetPlaceFactor(1);
		m_plawi->PlaceWidget(m_imageData->GetBounds());
		m_plawi->TubingOff();
		m_plawi->OutlineTranslationOff();
		m_plawi->DrawPlaneOff();
		m_plawi->SetDiagonalRatio(0.5);
		m_plawi->SetNormal(1, 0, 0);
		m_plawi->On();

		m_plawi->SetOrigin(m_imageData->GetCenter());

		//m_plawi->SetOrigin((bounds[0] + bounds[1]) / 2,
		//	(bounds[2] + bounds[3]) / 2, (bounds[4] + bounds[5]) / 2);

		//double* bv = m_bounds;
		////m_imageData->SetOrigin(bv[0], bv[2], bv[4]);

		//double* bi = m_imageData->GetBounds();
		//std::cout << "actor: " << bv[0] << "  " << bv[1] << "  " << bv[2]
		//	<< "  " << bv[3] << "  " << bv[4] << "  " << bv[5] << std::endl;
		//std::cout << "image: " << bi[0] << "  " << bi[1] << "  " << bi[2]
		//	<< "  " << bi[3] << "  " << bi[4] << "  " << bi[5] << std::endl;

		//double* norm = m_plawi->GetNormal();
		//std::cout << "Norm: " << norm[0] << "  " << norm[1] << "  " << norm[2] << std::endl;
		//double ele[16] = {
		//	norm[0], 0, 0, 0,
		//	norm[1], 1, 0, 0,
		//	norm[2], 0, 1, 0,
		//	0, 0, 0, 1 };
		//
		//auto transform = vtkSmartPointer<vtkTransform>::New();
		//
		//transform->PreMultiply();
		//transform->RotateY(45);
		//transform->RotateZ(50);
		//transform->RotateX(30);
		//double* pos = transform->TransformDoubleVector(norm);
		////double* pos = transform->TransformDoubleNormal(norm);
		////double* pos = transform->GetPosition();
		//std::cout << "Pos: " << pos[0] << "  " << pos[1] << "  " << pos[2] << std::endl;

		//m_plawi->SetNormal(pos);
		//m_imageData->Get

		//std::cout << bounds[0] << "  " << bounds[1]
		//	<< bounds[2] << "  " << bounds[3]
		//	<< bounds[4] << "  " << bounds[5] << std::endl;
		//m_plawi->SetOrigin(m_imageData->GetCenter());
		m_iren->Render();

		SetSliceView();

	}

	std::unique_ptr<double[]> SliceCouple::Rotate(double* norm, int dir, double angle)
	{
		auto pos_norm = std::unique_ptr<double[], std::default_delete<double[]>>(new double[3]);
		auto pnorm = pos_norm.get();
		double pi = 3.14159265359;
		double cosin = cos(angle*pi / 180);
		double sine = sin(angle*pi / 180);
		switch (dir)
		{
		case 1:
			pnorm[0] = norm[0];
			pnorm[1] = cosin*norm[1] - sine*norm[2];
			pnorm[2] = sine*norm[1] + cosin*norm[2];
			break;
		case 2:
			pnorm[0] = cosin*norm[0] + sine*norm[2];
			pnorm[1] = norm[1];
			pnorm[2] = -sine*norm[0] + cosin*norm[2];
			break;
		case 3:
			pnorm[0] = cosin*norm[0] - sine*norm[1];
			pnorm[1] = sine*norm[0] + cosin*norm[1];
			pnorm[2] = norm[2];
			break;
		default:
			pnorm[0] = norm[0];
			pnorm[1] = norm[1];
			pnorm[2] = norm[2];
			break;
		}
		return pos_norm;
		
	}

	void SliceCouple::SetSliceView()
	{
		int* dims = m_imageData->GetDimensions();
		//double* center = m_imageData->GetCenter();
		//double* spa = m_imageData->GetSpacing();
		m_left->SetSliceDirection(0);		
		//m_left->SetCenter(center[0], center[1], center[2]);
		m_left->SetSliceIndex(dims[0] / 2);
		m_right->SetSliceDirection(0);
		//m_right->SetCenter(center[0] + spa[0], center[1], center[2]);
		m_right->SetSliceIndex(dims[0] / 2 + 1);

		//auto renderer = vtkSmartPointer<vtkRenderer>::New();
		//auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
		//renWin->AddRenderer(renderer);
		//auto iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		//renWin->SetInteractor(iren);
		m_left->GetImagePlaneWidget()->SetInteractor(m_left->GetInteractor());
		m_right->GetImagePlaneWidget()->SetInteractor(m_right->GetInteractor());
		//iren->Initialize();

		//auto cbk = vtkSmartPointer<vtkSliceCoupleCallback>::New();
		//cbk->SetNandO(m_plawi->GetNormal(), m_plawi->GetOrigin());
		//cbk->m_couple = this;
		//m_plawi->GetInteractor()->AddObserver(
		//	vtkCommand::MouseMoveEvent, 
		//	cbk,m_plawi->GetPriority()+1);
		//m_plawi->GetInteractor()->AddObserver(
		//	vtkCommand::LeftButtonPressEvent,
		//	cbk, m_plawi->GetPriority() + 1);
		//m_plawi->GetInteractor()->AddObserver(
		//	vtkCommand::LeftButtonReleaseEvent,
		//	cbk, m_plawi->GetPriority() + 1);
		//m_plawi->AddObserver(vtkCommand::MouseMoveEvent, cbk);

		//vtkSmartPointer<vtkCallbackCommand> cllbk;
		//cllbk->SetCallback()

		m_left->SetImageData2(m_imageData);
		m_right->SetImageData2(m_imageData);

		auto ps = static_cast<vtkPlaneSource*>(m_left->GetImagePlaneWidget()
			->GetPolyDataAlgorithm());

		m_map = vtkSmartPointer<vtkPolyDataMapper>::New();
		m_map->SetInputConnection(ps->GetOutputPort());

		m_plane = vtkSmartPointer<vtkActor>::New();
		m_plane->SetMapper(m_map);
		this->m_plawi->GetCurrentRenderer()->AddActor(m_plane);

		this->m_iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(m_plane);
		
		this->Render();
		
		//double* cc = m_left->GetResliceCursor()->GetCenter();
		//double* cw = m_left->GetResliceCursorWidget()->GetResliceCursorRepresentation()
		//	->GetPlaneSource()->GetCenter();
		//double* nw = m_left->GetResliceCursorWidget()->GetResliceCursorRepresentation()
		//	->GetPlaneSource()->GetNormal();
		//double* nw2 = m_right->GetResliceCursorWidget()->GetResliceCursorRepresentation()
		//	->GetPlaneSource()->GetNormal();
		//std::cout << "should be center: " << center[0] << "  " << center[1] << "  " << center[2] << std::endl;
		//std::cout << "cursor center: " << cc[0] << "  " << cc[1] << "  " << cc[2] << std::endl;
		//std::cout << "widget center: " << cw[0] << "  " << cw[1] << "  " << cw[2] << std::endl;
		//std::cout << "widget normal: " << nw[0] << "  " << nw[1] << "  " << nw[2] << std::endl;
		//std::cout << "widget normal2: " << nw2[0] << "  " << nw2[1] << "  " << nw2[2] << std::endl;
	}
//}
//// 自己定义旋转函数
//// 平行于轴，单独调整角度

////切片显示