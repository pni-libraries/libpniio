//
// (c) Copyright 2018 DESY
//
// This file is part of libpniio.
//
// libpniio is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// libpniio is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libpniio.  If not, see <http://www.gnu.org/licenses/>.
// ===========================================================================
//
// Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
// Created on: Jan 9, 2018
//

#include <pni/io/nexus.hpp>
#include <h5cpp/hdf5.hpp>

using namespace pni::io;
using namespace pni::core;

class IsDataset : public nexus::NodePredicate
{
  public:

    virtual bool operator()(const hdf5::node::Node &node) const;
};

bool IsDataset::operator()(const hdf5::node::Node &node) const
{
  return node.type()==hdf5::node::Type::DATASET;
}

class Is1dDataset : public nexus::NodePredicate
{
  public:
    virtual bool operator()(const hdf5::node::Node &node) const;
};

bool Is1dDataset::operator()(const hdf5::node::Node &node) const
{
  if(node.type()!=hdf5::node::Type::DATASET)
  {
    return false;
  }

  hdf5::node::Dataset dataset(node);
  hdf5::Dimensions current_dimensions = nexus::get_dimensions(dataset);

  return (current_dimensions.size()==1) && (current_dimensions[0]>1);
}

int main(int argc,char **argv)
{
  //
  // handle command line arguments
  //
  boost::filesystem::path xml_file;
  if(argc<=1)
  {
    std::cerr<<"You have to provide a path to an XML file!"<<std::endl;
    return 1;
  }
  else
  {
    xml_file = argv[1];
    std::cout<<"Creating NeXus file from "<<xml_file<<std::endl;
  }


  hdf5::file::File file = nexus::create_file("find_datasets.nxs",hdf5::file::AccessFlags::TRUNCATE);
  hdf5::node::Group root_group = file.root();

  //create the file structure
  nexus::xml::create_from_file(root_group,xml_file);

  //
  // find all datasets in the file
  //
  std::cout<<"Found datasets:" <<std::endl;
  nexus::DatasetList datasets = nexus::search(root_group,IsDataset(),true);
  std::for_each(datasets.begin(),datasets.end(),
                [](const hdf5::node::Dataset &dataset)
                { std::cout<<dataset.link().path()<<std::endl;});

  //
  // find all 1d datasets in the file
  //
  std::cout<<"Found datasetes for plotting"<<std::endl;
  nexus::DatasetList plotable_data = nexus::search(root_group,Is1dDataset(),true);
  for(auto dataset: plotable_data)
  {
    hdf5::Dimensions dimensions = nexus::get_dimensions(dataset);
    std::cout<<dataset.link().path()<<" "<<nexus::get_type_id(dataset)
             <<" "<<dimensions[0]<<std::endl;
  }

  return 0;
}
