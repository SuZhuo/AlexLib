#include"vtkResliceCursorCallback.hpp"

namespace SliceLib
{
	void vtkResliceCursorCallback::Execute(vtkObject *caller,
		unsigned long eventId, void *callData)
	{
		auto rcw = dynamic_cast<vtkResliceCursorWidget*>(caller);
		if (rcw)
		{
			for (int i = 0; i < 3; i++)
			{
				auto ps = static_cast<vtkPlaneSource*>(view[i]->GetImagePlaneWidget()->GetPolyDataAlgorithm());
				ps->SetOrigin(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()
					->GetPlaneSource()->GetOrigin());
				ps->SetPoint1(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()
					->GetPlaneSource()->GetPoint1());
				ps->SetPoint2(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()
					->GetPlaneSource()->GetPoint2());

				view[i]->GetImagePlaneWidget()->UpdatePlacement();
				view[i]->Render();
			}
			view[3]->Render();
		}
	}
}