// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include <functional>

template <typename TSrc, typename TDest>
TArray<TDest> ConvertArrayElements(const TArray<TSrc> &Src, std::function<TDest(const TSrc &Src)> Convert)
{
    TArray<TDest> Dest;
    for (auto i = 0; i < Src.Num(); i++)
    {
        Dest.Add(Convert(Src[i]));
    }
    return Dest;
}

template <typename TSrc>
bool ValidateArrayElements(const TArray<TSrc> &Src, std::function<bool(const TSrc &Src)> Validate)
{
    for (auto i = 0; i < Src.Num(); i++)
    {
        if (!Validate(Src[i]))
        {
            return false;
        }
    }
    return true;
}

template <typename TKeySrc, typename TKeyDest, typename TValueSrc, typename TValueDest>
TMap<TKeyDest, TValueDest> ConvertMapElements(
    const TMap<TKeySrc, TValueSrc> &Src,
    std::function<TKeyDest(const TKeySrc &Src)> ConvertKey,
    std::function<TValueDest(const TValueSrc &Src)> ConvertValue)
{
    TMap<TKeyDest, TValueDest> Dest;
    for (auto KV : Src)
    {
        Dest.Add(ConvertKey(KV.Key), ConvertValue(KV.Value));
    }
    return Dest;
}

template <typename TKeySrc, typename TValueSrc>
bool ValidateMapElements(
    const TMap<TKeySrc, TValueSrc> &Src,
    std::function<bool(const TKeySrc &Src)> ValidateKey,
    std::function<bool(const TValueSrc &Src)> ValidateValue)
{
    for (auto KV : Src)
    {
        if (!ValidateKey(KV.Key) || !ValidateValue(KV.Value))
        {
            return false;
        }
    }
    return true;
}