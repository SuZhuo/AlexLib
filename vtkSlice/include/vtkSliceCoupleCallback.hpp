#pragma once

#include"vtkCommand.h"
#include"SliceCouple.hpp"
#include"vtkPlaneSource.h"
#include"vtkPlane.h"

namespace SliceLib
{
	class SliceCouple;

	class SLICELIB_API vtkSliceCoupleCallback : public vtkCommand
	{	
	public:
		vtkSliceCoupleCallback();

		void SetNandO(double*, double*);

		static vtkSliceCoupleCallback* New()
		{
			return new vtkSliceCoupleCallback;
		}
		void Execute(vtkObject *caller, unsigned long eventId, void *callData);

		SliceCouple* m_couple;
		enum Status
		{
			None,
			Rotating
		};
		Status m_state = None;
		double m_norm[3];
		double m_origin[3];

	private:
		bool IsChanged(double* norm, double* origin);
	};
}