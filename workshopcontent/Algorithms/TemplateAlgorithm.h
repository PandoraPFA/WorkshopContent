/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/TemplateAlgorithm.h
 * 
 *  @brief  Header file for the template algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_TEMPLATE_ALGORITHM_H
#define WORKSHOP_TEMPLATE_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  TemplateAlgorithm class
 */
class TemplateAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    // Member variables here
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *TemplateAlgorithm::Factory::CreateAlgorithm() const
{
    return new TemplateAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_TEMPLATE_ALGORITHM_H
