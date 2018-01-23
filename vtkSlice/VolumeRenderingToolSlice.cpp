#include"VolumeRenderingToolSlice.hpp"

VolumeRenderingToolSlice::VolumeRenderingToolSlice()
{
	m_opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	m_color = vtkSmartPointer<vtkColorTransferFunction>::New();
	m_prop = vtkSmartPointer<vtkVolumeProperty>::New();
	m_mapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
	m_mapper2D = vtkSmartPointer<vtkVolumeTextureMapper2D>::New();
	m_volume = vtkSmartPointer<vtkVolume>::New();
	//m_cast = vtkSmartPointer<vtkImageCast>::New();
	m_fun = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
}

void VolumeRenderingToolSlice::SetImageData(vtkSmartPointer<vtkImageData> imageData)
{
	m_imageData = imageData;
}

vtkSmartPointer<vtkImageData> VolumeRenderingToolSlice::GetImageData()
{
	return m_imageData;
}

vtkSmartPointer<vtkVolume> VolumeRenderingToolSlice::GetVolume()
{
	return m_volume;
}

void VolumeRenderingToolSlice::Update()
{
	//m_cast->SetInputData(m_imageData);
	//m_cast->SetOutputScalarTypeToUnsignedShort();
	//m_mapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
	//m_fun = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

	//设定体数据中不同标量值的透明度
	m_opacity->AddPoint(0, 0.0);
	m_opacity->AddPoint(150, 0.5);
	m_opacity->AddPoint(5000, 0.7);
	m_opacity->AddPoint(10000, 1.0);
	m_opacity->AddPoint(19000, 1.0);

	//设定不同标量值的颜色
	m_color->AddRGBPoint(0, 1, 0, 0);
	m_color->AddRGBPoint(3676, 1, 1, 0);
	m_color->AddRGBPoint(7352, 0.5, 1, 0);
	m_color->AddRGBPoint(11028, 0, 1, 0);
	m_color->AddRGBPoint(14704, 0, 1, 1);
	m_color->AddRGBPoint(19000, 0, 0, 1);

	m_prop->SetColor(m_color);
	m_prop->SetScalarOpacity(m_opacity);
	
	//设定插值类型为线性插值
	m_prop->SetInterpolationTypeToLinear();

	//m_mapper->SetSampleDistance(2.0);
	//m_mapper->SetInputConnection(m_cast->GetOutputPort());
	m_mapper->SetInputData(m_imageData);
	//m_mapper->RemoveAllInputConnections(0);
	m_mapper->SetVolumeRayCastFunction(m_fun);
	m_volume->SetMapper(m_mapper);
	//m_mapper2D->SetInputData(m_imageData);
	//m_volume->SetMapper(m_mapper2D);
	m_volume->SetProperty(m_prop);
	//m_volume->RotateY(180);
	//double* bv = m_volume->GetBounds();
	//
	//std::cout << "volume: " << bv[0] << "  " << bv[1] << "  " << bv[2]
	//	<< "  " << bv[3] << "  " << bv[4] << "  " << bv[5] << std::endl;
	//m_imageData->SetOrigin(bv[0], bv[2], bv[4]);
}

void VolumeRenderingToolSlice::Update2()
{

	//m_mapper2D = vtkSmartPointer<vtkVolumeTextureMapper2D>::New();
	//设定体数据中不同标量值的透明度
	m_opacity->AddPoint(0, 0.0);
	m_opacity->AddPoint(1, 1.0);
	//m_opacity->AddPoint(150, 0.5);
	//m_opacity->AddPoint(5000, 0.7);
	//m_opacity->AddPoint(10000, 1.0);
	m_opacity->AddPoint(65536, 1.0);

	//设定不同标量值的颜色
	m_color->AddRGBPoint(0, 1, 0, 0);
	m_color->AddRGBPoint(3676, 1, 1, 0);
	m_color->AddRGBPoint(7352, 0.5, 1, 0);
	m_color->AddRGBPoint(11028, 0, 1, 0);
	m_color->AddRGBPoint(14704, 0, 1, 1);
	m_color->AddRGBPoint(19000, 0, 0, 1);

	m_prop->SetColor(m_color);
	m_prop->SetScalarOpacity(m_opacity);

	//设定插值类型为线性插值
	m_prop->SetInterpolationTypeToLinear();

	//m_mapper->SetInputData(m_imageData);
	//m_mapper->SetVolumeRayCastFunction(m_fun);
	//m_mapper->SetAutoAdjustSampleDistances(0);
	//m_mapper->SetImageSampleDistance(0.5);
	//m_volume->SetMapper(m_mapper);
	m_mapper2D->SetInputData(m_imageData);
	m_volume->SetMapper(m_mapper2D);
	m_volume->SetProperty(m_prop);
}

void VolumeRenderingToolSlice::UpdateImage()//分情况讨论
{
	m_mapper->RemoveAllInputs();
	m_mapper->SetInputData(m_imageData);
	
}

void VolumeRenderingToolSlice::UpdateImageWithOutDelete()
{
	/*if (m_shiftfilter)
	{
		m_shiftfilter->RemoveAllInputs();
		m_shiftfilter->SetInputData(m_imageData);
	}*/
	/*double mini(0.0), maxi(0.0);
	int* ptr = (int*)(m_imageData->GetScalarPointer());
	double* range = m_imageData->GetScalarRange();
	mini = range[0];
	maxi = range[1];*/
	if (m_shiftfilter)
	{
		m_shiftfilter->Update();
	}
}

void VolumeRenderingToolSlice::UpdateWithCast()//mapper2D
{
	double mini(0.0), maxi(0.0);
	int* ptr = (int*)(m_imageData->GetScalarPointer());
	double* range = m_imageData->GetScalarRange();
	mini = range[0];
	maxi = range[1];
	//for (int i = 0; i < px_size; i++)
	//{
	//	int temp = *(ptr++);
	//	if (temp < mini&&temp>0) mini = temp;
	//	if (temp > maxi) maxi = temp;
	//}
	m_shiftfilter = vtkSmartPointer<vtkImageShiftScale>::New();
	m_shiftfilter->SetInputData(m_imageData);
	m_shiftfilter->SetOutputScalarTypeToUnsignedShort();
	m_shiftfilter->SetShift(-mini);
	m_shiftfilter->SetScale(65535.0 / (maxi - mini));
	m_shiftfilter->Update();

	m_mapper2D = vtkSmartPointer<vtkVolumeTextureMapper2D>::New();

	//设定体数据中不同标量值的透明度
	m_opacity->AddPoint(0, 0.0);
	//m_opacity->AddPoint(15000, 0.5);
	m_opacity->AddPoint(65536*0.5, 0.7);
	m_opacity->AddPoint(65536*0.8, 1.0);
	m_opacity->AddPoint(65536, 1.0);

	//设定不同标量值的颜色
	m_color->AddRGBPoint(0, 1, 0, 0);
	m_color->AddRGBPoint(65536*0.2, 1, 1, 0);
	m_color->AddRGBPoint(65536*0.4, 0.5, 1, 0);
	m_color->AddRGBPoint(65536*0.6, 0, 1, 0);
	m_color->AddRGBPoint(65536*0.8, 0, 1, 1);
	m_color->AddRGBPoint(65536, 0, 0, 1);

	m_prop->SetColor(m_color);
	m_prop->SetScalarOpacity(m_opacity);

	//设定插值类型为线性插值
	m_prop->SetInterpolationTypeToLinear();

	//m_mapper->SetSampleDistance(2.0);
	//m_mapper->SetInputConnection(m_cast->GetOutputPort());
	m_mapper2D->SetInputData(m_shiftfilter->GetOutput());
	//m_mapper->RemoveAllInputConnections(0);
	//m_mapper->SetVolumeRayCastFunction(m_fun);
	m_volume->SetMapper(m_mapper2D);
	//m_mapper2D->SetInputData(m_imageData);
	//m_volume->SetMapper(m_mapper2D);
	m_volume->SetProperty(m_prop);
	m_volume->RotateX(90);

}