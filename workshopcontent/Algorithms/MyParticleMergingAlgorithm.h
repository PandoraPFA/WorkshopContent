/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleMergingAlgorithm.h
 * 
 *  @brief  Header file for the myparticlemerging algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H
#define WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  MyParticleMergingAlgorithm class
 */
class MyParticleMergingAlgorithm : public pandora::Algorithm
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

inline pandora::Algorithm *MyParticleMergingAlgorithm::Factory::CreateAlgorithm() const
{
    return new MyParticleMergingAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H
