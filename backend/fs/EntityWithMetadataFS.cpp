// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "EntityWithMetadataFS.hpp"

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include "hdf5/SectionHDF5.hpp" // FIXME once the Section is implemented

#include <memory>
#include "SectionFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc)
    : NamedEntityFS(file, loc)
{
}

EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
	: EntityWithMetadataFS(file, bfs::path(loc)) 
{
}


EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc,
                                           const std::string &id, const std::string &type, const std::string &name)
    : EntityWithMetadataFS(file, loc, id, type, name, util::getTime())
{
}

EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc,
										   const std::string &id, const std::string &type, const std::string &name)
	: EntityWithMetadataFS(file, bfs::path(loc), id, type, name) 
{
}

EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc,
                                           const std::string &id, const std::string &type, const std::string &name,
                                           time_t time)
    : NamedEntityFS(file, loc, id, type, name, time)
{
}


EntityWithMetadataFS::EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc,
										   const std::string &id, const std::string &type, const std::string &name,
										   time_t time)
	: EntityWithMetadataFS(file, bfs::path(loc), id, type, name, time) 
{
}


void EntityWithMetadataFS::metadata(const std::string &id) {
    if (fileMode() == FileMode::ReadOnly) {
        throw std::runtime_error("EntityWithMetdata::metadata trying to set metadata in ReadOnly mode.");
    }
    if (id.empty())
        throw EmptyString("metadata");

    if (hasMetadata())
        metadata(none);
    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithMetadataFS::metadata: Section not found in file!");

    auto target = std::dynamic_pointer_cast<SectionFS>(found.front().impl());
    bfs::path t(target->location()), p(location()), m("metadata");
    target->createLink(p / m);
}


std::shared_ptr<base::ISection> EntityWithMetadataFS::metadata() const {
    std::shared_ptr<base::ISection> sec;
    if (hasMetadata()) {
        bfs::path p(location()), m("metadata"), other_loc(p/m);
        auto sec_tmp = std::make_shared<EntityWithMetadataFS>(file(), other_loc.string());
        // re-get above section "sec_tmp": we just got it to have id, parent is missing,
        // findSections will return it with parent!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }
    return sec;
}


void EntityWithMetadataFS::metadata(const none_t t) {
    if (fileMode() == FileMode::ReadOnly) {
        throw std::runtime_error("EntityWithMetdata::metadata trying to set metadata in ReadOnly mode.");
    }
    if (hasMetadata()) {
        bfs::path p(location()), m("metadata"), other_loc(p/m);
        auto sec_tmp = std::make_shared<EntityWithMetadataFS>(file(), other_loc.string());
        bfs::path p1(location()), p2("metadata");
        sec_tmp->unlink(p1 / p2);
        bfs::remove_all(p1/p2);
    }
    forceUpdatedAt();
}


bool EntityWithMetadataFS::hasMetadata() const {
    bfs::path p1(location()), p2("metadata");
    return bfs::exists(p1/p2);
}


EntityWithMetadataFS::~EntityWithMetadataFS() {}

} // ns nix::file
} // ns nix