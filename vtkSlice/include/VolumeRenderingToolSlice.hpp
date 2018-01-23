#pragma once

#include"SliceView.hpp"
#include"vtkVolumeRayCastMapper.h"
#include"vtkPiecewiseFunction.h"
#include"vtkColorTransferFunction.h"
#include"vtkVolumeRayCastCompositeFunction.h"
#include"vtkImageShiftScale.h"
#include"vtkVolumeProperty.h"
#include"vtkVolumeTextureMapper2D.h"

class VolumeRenderingToolSlice
{
public:
	VolumeRenderingToolSlice();

public:
	void VolumeRenderingToolSlice::SetImageData(vtkSmartPointer<vtkImageData> imageData);

	vtkSmartPointer<vtkVolume> VolumeRenderingToolSlice::GetVolume();

	void Update();
	void Update2();//减小采样距离 (可换成采用vtkVolumeTextureMapper2D)
	void UpdateImage();
	void UpdateWithCast();
	void UpdateImageWithOutDelete();
	vtkSmartPointer<vtkImageData> GetImageData();

private:
	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkVolume> m_volume;
	vtkSmartPointer<vtkVolumeRayCastMapper> m_mapper;
	vtkSmartPointer<vtkVolumeTextureMapper2D> m_mapper2D;
	//vtkSmartPointer<vtkImageCast> m_cast;

	vtkSmartPointer<vtkPiecewiseFunction> m_opacity;
	vtkSmartPointer<vtkColorTransferFunction> m_color;
	vtkSmartPointer<vtkVolumeProperty> m_prop;
	vtkSmartPointer<vtkVolumeRayCastCompositeFunction> m_fun;

	vtkSmartPointer<vtkImageShiftScale> m_shiftfilter;
};