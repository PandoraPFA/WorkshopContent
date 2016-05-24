/**
 *  @file   LArExampleContent/larexamplecontent/ExampleObjects/ExampleCaloHit.h
 * 
 *  @brief  Header file for the example calo hit class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_CALO_HIT_H
#define EXAMPLE_CALO_HIT_H 1

#include "Objects/CaloHit.h"

#include "Pandora/ObjectFactory.h"

#include "Persistency/BinaryFileReader.h"
#include "Persistency/BinaryFileWriter.h"
#include "Persistency/XmlFileReader.h"
#include "Persistency/XmlFileWriter.h"

#include <string>

/**
 *  @brief  Example calo hit parameters - simply add an additional property to the pandora calo hit
 */
class ExampleCaloHitParameters : public PandoraApi::CaloHit::Parameters
{
public:
    std::string     m_additionalProperty;       ///< The additional property string
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Example calo hit class - simply add an additional property to the pandora calo hit
 */
class ExampleCaloHit : public pandora::CaloHit
{
public:
    /**
     *  @brief  Constructor
     * 
     *  @param  parameters the example calo hit parameters
     */
    ExampleCaloHit(const ExampleCaloHitParameters &parameters);

    /**
     *  @brief  Get the additional property string
     * 
     *  @return the additional property string
     */
    const std::string &GetAdditionalProperty() const;

private:
    std::string     m_additionalProperty;       ///< The additional property string
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  CaloHitFactory responsible for default calo hit creation
 */
class ExampleCaloHitFactory : public pandora::ObjectFactory<PandoraApi::CaloHit::Parameters, pandora::CaloHit>
{
public:
    /**
     *  @brief  Create new parameters instance on the heap (memory-management to be controlled by user)
     * 
     *  @return the address of the new parameters instance
     */
    Parameters *NewParameters() const;

    /**
     *  @brief  Read any additional (derived class only) object parameters from file using the specified file reader
     *
     *  @param  parameters the parameters to pass in constructor
     *  @param  fileReader the file reader, used to extract any additional parameters from file
     */
    pandora::StatusCode Read(Parameters &parameters, pandora::FileReader &fileReader) const;

    /**
     *  @brief  Persist any additional (derived class only) object parameters using the specified file writer
     *
     *  @param  pObject the address of the object to persist
     *  @param  fileWriter the file writer
     */
    pandora::StatusCode Write(const pandora::CaloHit *const pObject, pandora::FileWriter &fileWriter) const;

    /**
     *  @brief  Create an object with the given parameters
     *
     *  @param  parameters the parameters to pass in constructor
     *  @param  pObject to receive the address of the object created
     */
    pandora::StatusCode Create(const PandoraApi::CaloHit::Parameters &parameters, const pandora::CaloHit *&pObject) const;
};

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

inline ExampleCaloHit::ExampleCaloHit(const ExampleCaloHitParameters &parameters) :
    pandora::CaloHit(parameters),
    m_additionalProperty(parameters.m_additionalProperty)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline const std::string &ExampleCaloHit::GetAdditionalProperty() const
{
    return m_additionalProperty;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

inline ExampleCaloHitFactory::Parameters *ExampleCaloHitFactory::NewParameters() const
{
    return (new ExampleCaloHitParameters);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleCaloHitFactory::Create(const Parameters &parameters, const pandora::CaloHit *&pObject) const
{
    const ExampleCaloHitParameters &exampleCaloHitParameters(dynamic_cast<const ExampleCaloHitParameters&>(parameters));
    pObject = new ExampleCaloHit(exampleCaloHitParameters);

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleCaloHitFactory::Read(Parameters &parameters, pandora::FileReader &fileReader) const
{
    // ATTN: To receive this call-back must have already set file reader calo hit factory to this factory
    std::string additionalProperty;

    if (pandora::BINARY == fileReader.GetFileType())
    {
        pandora::BinaryFileReader &binaryFileReader(dynamic_cast<pandora::BinaryFileReader&>(fileReader));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileReader.ReadVariable(additionalProperty));
    }
    else if (pandora::XML == fileReader.GetFileType())
    {
        pandora::XmlFileReader &xmlFileReader(dynamic_cast<pandora::XmlFileReader&>(fileReader));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileReader.ReadVariable("AdditionalProperty", additionalProperty));
    }
    else
    {
        return pandora::STATUS_CODE_INVALID_PARAMETER;
    }

    ExampleCaloHitParameters &exampleCaloHitParameters(dynamic_cast<ExampleCaloHitParameters&>(parameters));
    exampleCaloHitParameters.m_additionalProperty = additionalProperty;

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleCaloHitFactory::Write(const pandora::CaloHit *const pObject, pandora::FileWriter &fileWriter) const
{
    // ATTN: To receive this call-back must have already set file writer calo hit factory to this factory
    const ExampleCaloHit *const pExampleCaloHit(dynamic_cast<const ExampleCaloHit*>(pObject));

    if (!pExampleCaloHit)
        return pandora::STATUS_CODE_INVALID_PARAMETER;

    if (pandora::BINARY == fileWriter.GetFileType())
    {
        pandora::BinaryFileWriter &binaryFileWriter(dynamic_cast<pandora::BinaryFileWriter&>(fileWriter));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileWriter.WriteVariable(pExampleCaloHit->GetAdditionalProperty()));
    }
    else if (pandora::XML == fileWriter.GetFileType())
    {
        pandora::XmlFileWriter &xmlFileWriter(dynamic_cast<pandora::XmlFileWriter&>(fileWriter));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileWriter.WriteVariable("AdditionalProperty", pExampleCaloHit->GetAdditionalProperty()));
    }
    else
    {
        return pandora::STATUS_CODE_INVALID_PARAMETER;
    }

    return pandora::STATUS_CODE_SUCCESS;
}

#endif // #ifndef EXAMPLE_CALO_HIT_H
