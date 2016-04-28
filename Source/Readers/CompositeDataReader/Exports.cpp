//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// DataReader.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Basics.h"

#define DATAREADER_EXPORTS
#include "DataReader.h"
#include "CompositeDataReader.h"
#include "ReaderShim.h"
#include "HeapMemoryProvider.h"

namespace Microsoft { namespace MSR { namespace CNTK {

template<class TElement>
class CompositeReaderShim : ReaderShim<TElement>
{
public:
    explicit CompositeReaderShim(ReaderFactory f) : ReaderShim(f) {}

    // Returning 0 for composite configs.
    // This forbids the use of learning-rate and momentum per MB if truncation is enabled.
    size_t GetNumParallelSequences() override
    {
        return 0;
    }
};

auto factory = [](const ConfigParameters& parameters) -> ReaderPtr
{
    return std::make_shared<CompositeDataReader>(parameters, std::make_shared<HeapMemoryProvider>());
};

extern "C" DATAREADER_API void GetReaderF(IDataReader** preader)
{
    *preader = new ReaderShim<float>(factory);
}

extern "C" DATAREADER_API void GetReaderD(IDataReader** preader)
{
    *preader = new ReaderShim<double>(factory);
}

}}}
