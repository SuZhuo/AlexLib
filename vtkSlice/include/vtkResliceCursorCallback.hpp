
#pragma once

#include"vtkCommand.h"
#include"SliceView.hpp"
#include"vtkPlaneSource.h"

namespace SliceLib
{
	class SLICELIB_API vtkResliceCursorCallback : public vtkCommand
	{
	public:
		static vtkResliceCursorCallback* New()
		{
			return new vtkResliceCursorCallback;
		}
		void Execute(vtkObject *caller, unsigned long eventId, void *callData);

		SliceView* view[4];
	};
}

