// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/SourceHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


SourceHDF5::SourceHDF5(const SourceHDF5 &source)
    : EntityWithMetadataHDF5(source.file(), source.group(), source.id())
{
    source_group = source.source_group;
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id)
    : EntityWithMetadataHDF5(file, group, id)
{
    source_group = group.openGroup("sources");
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id, time_t time)
    : EntityWithMetadataHDF5(file, group, id, time)
{
    source_group = group.openGroup("sources");
}

/*
SourceHDF5::size_type SourceHDF5::childCount() const {
    return source_group.objectCount();
}


Source SourceHDF5::getChild(size_type index) const {
    string id = source_group.objectName(index);
    return Source(file, source_group.openGroup(id, false), id);
}*/


bool SourceHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


Source SourceHDF5::getSource(const string &id) const {
    auto tmp = shared_ptr<SourceHDF5>(new SourceHDF5(file(), source_group.openGroup(id, false), id));
    return Source(tmp);
}


Source SourceHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    auto tmp = shared_ptr<SourceHDF5>(new SourceHDF5(file(), source_group.openGroup(id, false), id));
    return Source(tmp);
}


size_t SourceHDF5::sourceCount() const {
    return source_group.objectCount();
}


std::vector<Source> SourceHDF5::sources() const {
    vector<Source> srcs;
    // TODO implement
    // return collectIf(predCollectAll, true, 1);
    return srcs;
}


Source SourceHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    auto tmp = shared_ptr<SourceHDF5>(new SourceHDF5(file(), source_group.openGroup(id, true), id));
    tmp->name(name);
    tmp->type(type);

    return Source(tmp);
}


bool SourceHDF5::removeSource(const string &id) {
    if (source_group.hasGroup(id)) {
        source_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


ostream& operator<<(ostream &out, const SourceHDF5 &ent) {
    out << "Source: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


void SourceHDF5::swap(SourceHDF5 &other) {
    using std::swap;

    EntityWithMetadataHDF5::swap(other);
    swap(source_group, other.source_group);
}


SourceHDF5& SourceHDF5::operator=(const SourceHDF5 &other) {
    if (*this != other) {
        SourceHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


SourceHDF5::~SourceHDF5() {}


} // namespace hdf5
} // namespace nix