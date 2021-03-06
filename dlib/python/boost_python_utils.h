// Copyright (C) 2013  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_BOOST_PYTHON_UtILS_Hh_
#define DLIB_BOOST_PYTHON_UtILS_Hh_

#include <boost/python.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <vector>
#include <string>
#include <dlib/serialize.h>

namespace py = pybind11;


namespace pybind11 {
// https://sourceforge.net/p/ngsolve/git/ci/master/tree/ngstd/python_ngstd.hpp#l45
template<typename T>
bool CheckCast( py::handle obj ) {
  try{
    obj.cast<T>();
    return true;
  }
  catch (py::cast_error &e) {
    return false;
  }
  catch (py::error_already_set &e) {
    return false;
  }
}

template <typename T>
struct extract
{
py::handle obj;
extract( py::handle aobj ) : obj(aobj) {}

bool check() { return CheckCast<T>(obj); }
T operator()() { return obj.cast<T>(); }
};

}

inline bool hasattr(
    py::object obj, 
    const std::string& attr_name
) 
/*!
    ensures
        - if (obj has an attribute named attr_name) then
            - returns true
        - else
            - returns false
!*/
{
     return PyObject_HasAttrString(obj.ptr(), attr_name.c_str());
}

// ----------------------------------------------------------------------------------------

// python list <-> cpp vector automatique conversion by pybind11
// template <typename T>
// std::vector<T> python_list_to_vector (
//     const py::object& obj
// )
// /*!
//     ensures
//         - converts a python object into a std::vector<T> and returns it.
// !*/
// {
//     std::vector<T> vect(len(obj));
//     for (unsigned long i = 0; i < vect.size(); ++i)
//     {
//         vect[i] = obj[i].cast<T>()
//     }
//     return vect;
// }

// template <typename T>
// py::list vector_to_python_list (
//     const std::vector<T>& vect
// )
// /*!
//     ensures
//         - converts a std::vector<T> into a python list object.
// !*/
// {
//     py::list obj;
//     for (unsigned long i = 0; i < vect.size(); ++i)
//         obj.append(vect[i]);
//     return obj;
// }

// ----------------------------------------------------------------------------------------

template <typename T>
std::shared_ptr<T> load_object_from_file (
    const std::string& filename
)
/*!
    ensures
        - deserializes an object of type T from the given file and returns it.
!*/
{
    std::ifstream fin(filename.c_str(), std::ios::binary);
    if (!fin)
        throw dlib::error("Unable to open " + filename);
    std::shared_ptr<T> obj(new T());
    deserialize(*obj, fin);
    return obj;
}

// ----------------------------------------------------------------------------------------


#endif // DLIB_BOOST_PYTHON_UtILS_Hh_

