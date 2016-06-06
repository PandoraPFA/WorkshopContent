#!/usr/bin/python

import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--name", help="New algorithm name")
args = parser.parse_args()

if args.name:
    newName = str(args.name)
    print '\nNew Algorithm name: ' + newName + 'Algorithm'
else:
    print args.echo
    exit

for extension in ['cc', 'h']:
    inputFile = open('TemplateAlgorithm.' + extension, 'r')
    inputFiledata = inputFile.read()
    inputFile.close()

    updatedData = inputFiledata.replace("Template", newName)
    updatedData = updatedData.replace("TEMPLATE", newName.upper())
    updatedData = updatedData.replace("template", newName.lower())

    outputFile = open(newName + 'Algorithm.' + extension, 'w')
    outputFile.write(updatedData)
    outputFile.close()

includeInput = '#include "workshopcontent/Algorithms/TemplateAlgorithm.h"'
registerAlgorithm = 'PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora, "Template", new workshop_content::TemplateAlgorithm::Factory));'
updatedInclude = includeInput.replace("Template", newName)
updatedRegister = registerAlgorithm.replace("Template", newName)

print '\nin workshopcontent/Test/PandoraWorkshop.cc\n'
print '\t' + updatedInclude + '\n'
print '\t' + updatedRegister + '\n'
