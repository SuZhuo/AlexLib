#AlexLib

The user defined VTK widget inspired from vtkImplicitPlaneWidget for manipulating an infinite plane.
This widget consists of a plane representing the slice of a object, and a normal vector controlling the direction of the slice. The slice can be in any position with any direction within the object, this is just controlled by a simple click and movement of the cursor. In addition, there are two seprate windows simultaneously render both sides of the slice.
The main class in: SliceCouple.hpp

Environment:
VS 2013

language: C++

VTK 6.3.0


Similar to vtkImplicitPlaneWidget, using the following interface to apply the widget:

		void PreInit(); //initialize the rendering windows
		void SetImage(vtkSmartPointer<vtkImageData> imageData); // input the slice data connecting to the redering plane
		void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> iren); //set interactor of the rendering scene
		void Update(int dir, double angle, bool reset = false); //update the direction
		void TurnOn(); //provoke the widget
		void TurnOff(); //switch off the widget
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
