#include"vtkSliceCoupleCallback.hpp"

namespace SliceLib
{
	vtkSliceCoupleCallback::vtkSliceCoupleCallback()
	{
		m_norm[0] = m_norm[1] = m_norm[2] = 0;
		m_origin[0] = m_origin[1] = m_origin[2] = 0;
	}

	bool vtkSliceCoupleCallback::IsChanged(double* norm, double* origin)
	{
		if (m_origin[0] != origin[0] || m_origin[1] != origin[1] || m_origin[2] != origin[2])
			return true;
		if (m_norm[0] != norm[0] || m_norm[1] != norm[1] || m_norm[2] != norm[2])
			return true;
		return false;
	}

	void vtkSliceCoupleCallback::SetNandO(double* norm, double* origin)
	{
		m_norm[0] = norm[0]; m_norm[1] = norm[1]; m_norm[2] = norm[2];
		m_origin[0] = origin[0]; m_origin[1] = origin[1]; m_origin[2] = origin[2];
	}

	void vtkSliceCoupleCallback::Execute(vtkObject *caller, 
		unsigned long eventId, void *callData)
	{
		switch (eventId)
		{
		case vtkCommand::LeftButtonPressEvent:
		{
			//std::cout << "haha" << std::endl;
			double* pn = m_couple->m_plawi->GetNormal();
			double* po = m_couple->m_plawi->GetOrigin();
			//std::cout << "normal: " << pn[0] << "  " << pn[1] << "  "
			//	<< pn[2] << "  " << std::endl;
			//std::cout << "original: " << po[0] << "  " << po[1] << "  "
			//	<< po[2] << "  " << std::endl << std::endl;
			m_state = Rotating;
			break;
		}		
		case vtkCommand::MouseMoveEvent:
		{
			if (Rotating != m_state) break;
			//auto dst = static_cast<vtkPlaneSource*>(m_couple->m_plawi->GetPolyDataAlgorithm());
			//auto plane = vtkSmartPointer<vtkPlane>::New();
			//m_couple->m_plawi->GetPlane(plane);
			std::cout << "m_plami:" << std::endl;
			double* pn = m_couple->m_plawi->GetNormal();
			double* po = m_couple->m_plawi->GetOrigin();
			if (!IsChanged(pn,po)) break;
			std::cout << "normal: " << pn[0] << "  " << pn[1] << "  "
				<< pn[2] << "  " << std::endl;
			std::cout << "original: " << po[0] << "  " << po[1] << "  "
				<< po[2] << "  " << std::endl << std::endl;

			auto dst = vtkSmartPointer<vtkPlaneSource>::New();
			//dst->SetInputData(m_couple->m_plawi->GetPolyData());
			//dst->SetResolution(m_couple->m_plawi->Get)
			dst->SetNormal(m_couple->m_plawi->GetNormal());
			dst->SetOrigin(m_couple->m_plawi->GetOrigin());

			m_couple->m_left->GetImagePlaneWidget()->SetOrigin(dst->GetOrigin());
			m_couple->m_left->GetImagePlaneWidget()->SetPoint1(dst->GetPoint1());
			m_couple->m_left->GetImagePlaneWidget()->SetPoint2(dst->GetPoint2());

			m_couple->m_right->GetImagePlaneWidget()->SetOrigin(dst->GetOrigin());
			m_couple->m_right->GetImagePlaneWidget()->SetPoint1(dst->GetPoint1());
			m_couple->m_right->GetImagePlaneWidget()->SetPoint2(dst->GetPoint2());

			m_couple->m_left->GetImagePlaneWidget()->UpdatePlacement();
			m_couple->m_right->GetImagePlaneWidget()->UpdatePlacement();

			//m_couple->m_left->RotateTo(m_couple->m_plawi->GetNormal());
			//m_couple->m_right->RotateTo(m_couple->m_plawi->GetNormal());

			m_couple->m_left->Render();
			m_couple->m_right->Render();

			std::cout << "m_left:" << std::endl;
			pn = m_couple->m_left->GetImagePlaneWidget()->GetNormal();
			po = m_couple->m_left->GetImagePlaneWidget()->GetOrigin();
			std::cout << "normal: " << pn[0] << "  " << pn[1] << "  "
				<< pn[2] << "  " << std::endl;
			std::cout << "original: " << po[0] << "  " << po[1] << "  "
				<< po[2] << "  " << std::endl << std::endl;
			break;
		}			
		case vtkCommand::LeftButtonReleaseEvent:
			if (Rotating == m_state) m_state = None;
			break;
		default:
			break;
		}

	}
}