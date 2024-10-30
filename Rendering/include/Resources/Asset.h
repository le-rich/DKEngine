#pragma once

#include "Utils/IDUtils.h"

class Asset
{
protected:
    // Unique id for asset
    UUIDv4::UUID assetID;

    // Changeable display name for asset
    std::string assetDisplayName;

public:
    Asset();
    Asset(std::string pDisplayName);
    ~Asset() {}

    // Retrieve id of asset
    inline const UUIDv4::UUID GetAssetID() { return assetID; }

    // Retrieve display name of asset
    inline std::string GetDisplayName() { return assetDisplayName; }

    // Set the asset uuid
    inline void SetAsetID(const UUIDv4::UUID& pNewID) { this->assetID = pNewID; }

    // Set display name of asset
    inline void SetDisplayName(std::string pNewID) { assetDisplayName = pNewID; }

    bool operator==(const Asset& other) const
    {
        return (other.assetID == this->assetID &&
            other.assetDisplayName == this->assetDisplayName);
    }
};

