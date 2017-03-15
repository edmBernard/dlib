// Copyright (C) 2013  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include <dlib/python.h>
#include <memory>
#include <dlib/matrix.h>
// #include <boost/python/slice.hpp>
#include <dlib/geometry/vector.h>
#include "indexing.h"


using namespace dlib;
using namespace std;
namespace py = pybind11;

typedef matrix<double,0,1> cv;

void cv_set_size(cv& m, long s)
{
    m.set_size(s);
    m = 0;
}

double dotprod ( const cv& a, const cv& b)
{
    return dot(a,b);
}

string cv__str__(const cv& v)
{
    ostringstream sout;
    for (long i = 0; i < v.size(); ++i)
    {
        sout << v(i);
        if (i+1 < v.size())
            sout << "\n";
    }
    return sout.str();
}

string cv__repr__ (const cv& v)
{
    std::ostringstream sout;
    sout << "dlib.vector([";
    for (long i = 0; i < v.size(); ++i)
    {
        sout << v(i);
        if (i+1 < v.size())
            sout << ", ";
    }
    sout << "])";
    return sout.str();
}

std::shared_ptr<cv> cv_from_object(py::object obj)
{
    py::extract<long> thesize(obj);
    if (thesize.check())
    {
        long nr = thesize();
        std::shared_ptr<cv> temp(new cv(nr));
        *temp = 0;
        return temp;
    }
    else
    {
        const long nr = len(obj);
        std::shared_ptr<cv> temp(new cv(nr));
        for ( long r = 0; r < nr; ++r)
        {
            (*temp)(r) = py::extract<std::vector<double>>(obj)()[r];  // automatic conversion in vector by pybind11 I hope not the same thing than boost
        }
        return temp;
    }
}

long cv__len__(cv& c)
{
    return c.size();
}


void cv__setitem__(cv& c, long p, double val)
{
    if (p < 0) {
        p = c.size() + p; // negative index
    }
    if (p > c.size()-1) {
        PyErr_SetString( PyExc_IndexError, "index out of range"
        );
        boost::python::throw_error_already_set();
    }
    c(p) = val;
}

double cv__getitem__(cv& m, long r)
{
    if (r < 0) {
        r = m.size() + r; // negative index
    }
    if (r > m.size()-1 || r < 0) {
        PyErr_SetString( PyExc_IndexError, "index out of range"
        );
        boost::python::throw_error_already_set();
    }
    return m(r);
}


cv cv__getitem2__(cv& m, py::slice r)
{
    // boost::python::slice::range<cv::iterator> bounds;
    // bounds = r.get_indicies<>(m.begin(), m.end());
    // long num = (bounds.stop-bounds.start+1);
    // // round num up to the next multiple of bounds.step.
    // if ((num%bounds.step) != 0)
    //     num += bounds.step - num%bounds.step;

    // cv temp(num/bounds.step);

    // if (temp.size() == 0)
    //     return temp;
    // long ii = 0;
    // while(bounds.start != bounds.stop)
    // {
    //     temp(ii++) = *bounds.start;
    //     std::advance(bounds.start, bounds.step);
    // }
    // temp(ii) = *bounds.start;
    // return temp;

    // from pybind11 test_sequences_and_iterators TODO: check if it really work
    size_t start, stop, step, slicelength;
    if (!r.compute(m.size(), &start, &stop, &step, &slicelength))
        throw py::error_already_set();
    cv *seq = new cv(slicelength);
    for (size_t i=0; i<slicelength; ++i) {
        (*seq)(i) = m(start); 
        start += step;
    }
    return *seq;
}


py::tuple cv_get_matrix_size(cv& m)
{
    return py::make_tuple(m.nr(), m.nc());
}

// ----------------------------------------------------------------------------------------

string point__repr__ (const point& p)
{
    std::ostringstream sout;
    sout << "point(" << p.x() << ", " << p.y() << ")";
    return sout.str();
}

string point__str__(const point& p)
{
    std::ostringstream sout;
    sout << "(" << p.x() << ", " << p.y() << ")";
    return sout.str();
}

long point_x(const point& p) { return p.x(); }
long point_y(const point& p) { return p.y(); }

// ----------------------------------------------------------------------------------------
void bind_vector(py::module& m)
{
    {
    py::class_<cv>(m, "vector", "This object represents the mathematical idea of a column vector.")
        // .def(py::init<py::object>())  // implicite conversion between py::object and vector
        .def("__init__", &cv_from_object)
        .def("set_size", &cv_set_size)
        .def("resize", &cv_set_size)
        .def("__repr__", &cv__repr__)
        .def("__str__", &cv__str__)
        .def("__len__", &cv__len__)
        .def("__getitem__", &cv__getitem__)
        .def("__getitem__", &cv__getitem2__)
        .def("__setitem__", &cv__setitem__)
        .def("shape", &cv_get_matrix_size)  // TODO: verify add_property but that only a getter
        // .def_pickle(serialize_pickle<cv>());
        ;

    m.def("dot", dotprod, "Compute the dot product between two dense column vectors.");
    }
    {
    typedef point type;
    py::class_<type>(m, "point", "This object represents a single point of integer coordinates that maps directly to a dlib::point.")
        .def(py::init<long, long>(), py::arg("x"), py::arg("y"))
        .def("__repr__", &point__repr__)
        .def("__str__", &point__str__)
        .def("x", &point_x, "The x-coordinate of the point.")   // TODO: verify add_property but that only a getter
        .def("y", &point_y, "The y-coordinate of the point.")   // TODO: verify add_property but that only a getter
        // .def_pickle(serialize_pickle<type>())
        ;
    }
    {
    typedef std::vector<point> type;
    py::class_<type>(m, "points", "An array of point objects.")
        .def(py::init<type>())
        .def("clear", &type::clear)
        .def("resize", resize<type>)
        // .def_pickle(serialize_pickle<type>())
        ;
    }
}
