// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_HDF5_INCLUDE_HPP
#define NIX_HDF5_INCLUDE_HPP

#include <hdf5.h>

#if defined(_WIN32)
#include <cpp/H5Cpp.h>
#else
#include <H5Cpp.h>
#endif

#endif // NIX_HDF5_INCLUDE_HPP