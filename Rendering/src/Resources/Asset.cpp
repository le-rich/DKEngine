#include "Resources/Asset.h"

Asset::Asset()
{
    this->assetID = uuidGen.getUUID();
}

Asset::Asset(std::string pDisplayName)
{
    this->assetID = uuidGen.getUUID();
    this->assetDisplayName = pDisplayName;
}