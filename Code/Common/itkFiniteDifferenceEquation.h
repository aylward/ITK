/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAcosImageAdaptor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkFiniteDifferenceEquation_h_
#define __itkFiniteDifferenceEquation_h_

#include "itkLightObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstSmartNeighborhoodIterator.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk {

/**
 * \class FiniteDifferenceEquation
 *
 * \warning  The Evaluate() methods are declared as const to enforce
 *  thread-safety during execution of FiniteDifferenceImageFilter
 *  algorithms.
 */
template<class TImageType>
class FiniteDifferenceEquation : public LightObject
{
public:

 /**
   * Standard itk Self & Superclass typedefs
   */
  typedef FiniteDifferenceEquation Self;
  typedef LightObject Superclass;

  /**
   * Extract some parameters from the image type
   */
  typedef TImageType ImageType;
  enum { ImageDimension = ImageType::ImageDimension };
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::ScalarValueType TimeStepType;
  typedef typename ImageType::ScalarValueType ScalarValueType;

  /**
   * The default boundary condition for finite difference
   * functions that is used unless overridden in the Evaluate() method.
   */
  typedef ZeroFluxNeumannBoundaryCondition<ImageType>
    DefaultBoundaryConditionType;

  /** 
   * Smart pointer support for this class.
   */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro( FiniteDifferenceEquation, LightObject );

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Neighborhood radius type
   */
  typedef typename ConstNeighborhoodIterator<TImageType>::RadiusType RadiusType;

  /**
   * 
   */
  typedef ConstNeighborhoodIterator<TImageType> NeighborhoodType;

  /**
   *
   */
  typedef ConstSmartNeighborhoodIterator<TImageType,
    DefaultBoundaryConditionType> BoundaryNeighborhoodType;

  /**
   *
   */
  virtual void InitializeIteration() {};

  /**
   *
   */
  virtual PixelType  ComputeUpdate(const NeighborhoodType &neighborhood,
                               TimeStepType &dt) const = 0;

  /**
   *
   */
  virtual PixelType  ComputeUpdate(const BoundaryNeighborhoodType
                               &neighborhood, TimeStepType &dt) const = 0;

  /**
   *  
   */
  void SetRadius(const RadiusType &r)
    {      m_Radius = r;    }

  /**
   *
   */
  const RadiusType &GetRadius() const
    {      return m_Radius;    }

  /**
   * 
   */
  virtual TimeStepType GetInitialTimeStep() const = 0;

protected:
  FiniteDifferenceEquation() {}
  ~FiniteDifferenceEquation() {}
  FiniteDifferenceEquation(const Self&) {}
  void operator=(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent)
  {
    os << indent << "FiniteDifferenceEquation";
    Superclass::PrintSelf(os, indent.GetNextIndent() );
  }
  
  RadiusType m_Radius;
};

} // end namespace itk


#endif
