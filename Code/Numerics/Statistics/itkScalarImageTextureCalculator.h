#ifndef __itkScalarImageTextureCalculator_h
#define __itkScalarImageTextureCalculator_h

#include "itkImage.h"
#include "itkObject.h"
#include "itkVectorContainer.h"
#include "itkMacro.h"

#include "itkGreyLevelCooccurrenceMatrixTextureCoefficientsCalculator.h"
#include "itkMaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator.h"

namespace itk {
  namespace Statistics {

/** \class ScalarImageTextureCalculator 
*  \brief This class computes texture descriptions from an image.
*
* This class computes features that summarize the texture of a given image.
* The texture features are compute a la Haralick, and have proven to be useful
* in image classification for biological and medical imaging.
* This class computes the texture features of an image (optionally in a masked
* masked region), averaged across several spatial directions so that they are 
* invariant to rotation.
*
* This class is templated over the input image type.
  
* Inputs and parameters:
* (1) An image
* (2) A mask defining the region over which texture features will be
* calculated. (Optional)
* (2a) The pixel value that defines the "inside" of the mask. (Optional, defaults
* to 1 if a mask is set.)
* (3) The set of features to be calculated. These features are defined
* in the GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator class. (Optional,
* defaults to {Energy, Entropy, InverseDifferenceMoment, Inertia, ClusterShade, 
* ClusterProminence}, as in Conners, Trivedi and Harlow.)
* (4) The number of intensity bins. (Optional, defaults to 256.)
* (5) The set of directions (offsets) to average across. (Optional, defaults to
* {(-1, 0), (-1, -1), (0, -1), (1, -1)} for 2D images and scales analogously for ND 
* images.)
* (6) The pixel intensity range over which the features will be calculated.
* (Optional, defaults to the full dynamic range of the pixel type.)
* 
* In general, the default parameter values should be sufficient.
*
* Outputs:
* (1) The average value of each feature.
* (2) The standard deviation in the values of each feature.
* 
* Web references:
* http://www.cssip.uq.edu.au/meastex/www/algs/algs/algs.html
* http://www.ucalgary.ca/~mhallbey/texture/texture_tutorial.html
*
* Print references:
* Haralick, R.M., K. Shanmugam and I. Dinstein. 1973.  Textural Features for 
* Image Classification. IEEE Transactions on Systems, Man and Cybernetics. 
* SMC-3(6):610-620.
*
* Haralick, R.M. 1979. Statistical and Structural Approaches to Texture. 
* Proceedings of the IEEE, 67:786-804.
*
* R.W. Conners and C.A. Harlow. A Theoretical Comaprison of Texture Algorithms. 
* IEEE Transactions on Pattern Analysis and Machine Intelligence,  2:204-222, 1980.
*
* R.W. Conners, M.M. Trivedi, and C.A. Harlow. Segmentation of a High-Resolution
* Urban Scene using Texture  Operators. Computer Vision, Graphics and Image 
* Processing, 25:273-310,  1984.
* 
* \sa ScalarImageToGreyLevelCooccurrenceMatrixGenerator
* \sa MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
* \sa GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator
*
* Author: Zachary Pincus
*/
    
  template< class TImageType >
  class ScalarImageTextureCalculator : public Object
    {
    public:
      /** Standard typedefs */
      typedef ScalarImageTextureCalculator Self;
      typedef Object Superclass;
      typedef SmartPointer<Self> Pointer;
      typedef SmartPointer<const Self> ConstPointer;
      
      /** Run-time type information (and related methods). */
      itkTypeMacro(ScalarImageTextureCalculator, Object);
      
      /** standard New() method support */
      itkNewMacro(Self) ;
      
      typedef TImageType                                  ImageType;
      typedef typename ImageType::Pointer                 ImagePointer;
      
      typedef typename ImageType::PixelType               PixelType;
      typedef typename ImageType::OffsetType              OffsetType;
      typedef VectorContainer<unsigned char, OffsetType>  OffsetVector;
      typedef typename OffsetVector::Pointer              OffsetVectorPointer;

      typedef MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator< ImageType >
                                                          GLCMGeneratorType;
      typedef GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator< typename
        GLCMGeneratorType::HistogramType >                GLCMCalculatorType;
      
      typedef VectorContainer<unsigned char, TextureFeatureName> FeatureNameVector;
      typedef typename FeatureNameVector::Pointer         FeatureNameVectorPointer;
      typedef VectorContainer<unsigned char, double>      FeatureValueVector;
      typedef typename FeatureValueVector::Pointer        FeatureValueVectorPointer;
      
     
      
      /** Triggers the computation of the features */
      void Compute( void );
      
      /** Connects the input image for which the features are going to be computed */
      void SetInput( const ImagePointer );
      
      /** Return the feature means and deviations.
        \warning This output is only valid after the Compute() method has been invoked 
        \sa Compute */
      itkGetMacro(FeatureMeans, FeatureValueVectorPointer);
      itkGetMacro(FeatureStandardDeviations, FeatureValueVectorPointer);
      
      /** Set the desired feature set. Optional, for default value see above. */
      itkSetMacro(RequestedFeatures, FeatureNameVectorPointer);
      
      /** Set the  offsets over which the co-occurrence pairs will be computed.
        Optional; for default value see above. */
      itkSetMacro(Offsets, OffsetVectorPointer);      
      
      /** Set number of histogram bins along each axis. 
        Optional; for default value see above. */
      void SetNumberOfBinsPerAxis( unsigned int numberOfBins );
      
      /** Set the min and max (inclusive) pixel value that will be used for
        feature calculations. Optional; for default value see above. */
      void SetPixelValueMinMax( PixelType min, PixelType max );
      
      /** Connects the mask image for which the histogram is going to be computed.
        Optional; for default value see above. */
      void SetImageMask(ImagePointer ImageMask);
      
      /** Set the pixel value of the mask that should be considered "inside" the 
        object. Optional; for default value see above. */
      void SetInsidePixelValue(PixelType InsidePixelValue);
      
      
    protected:
      ScalarImageTextureCalculator();
      virtual ~ScalarImageTextureCalculator() {};
      void PrintSelf(std::ostream& os, Indent indent) const;
      
    private:
      typename GLCMGeneratorType::Pointer m_GLCMGenerator;
      FeatureValueVectorPointer m_FeatureMeans, m_FeatureStandardDeviations;
      FeatureNameVectorPointer m_RequestedFeatures;
      OffsetVectorPointer m_Offsets;
    };    
    
  } // end of namespace Statistics 
} // end of namespace itk 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarImageTextureCalculator.txx"
#endif

#endif
