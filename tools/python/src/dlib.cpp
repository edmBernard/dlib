// Copyright (C) 2015 Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include <boost/python.hpp>
#include <pybind11/pybind11.h>
namespace py = pybind11;

void bind_matrix(py::model& m);
void bind_vector(py::model& m);
void bind_svm_c_trainer(py::model& m);
void bind_decision_functions(py::model& m);
void bind_basic_types(py::model& m);
void bind_other(py::model& m);
void bind_svm_rank_trainer(py::model& m);
void bind_cca(py::model& m);
void bind_sequence_segmenter(py::model& m);
void bind_svm_struct(py::model& m);
void bind_image_classes(py::model& m);
void bind_rectangles(py::model& m);
void bind_object_detection(py::model& m);
void bind_shape_predictors(py::model& m);
void bind_correlation_tracker(py::model& m);
void bind_face_recognition(py::model& m);

#ifndef DLIB_NO_GUI_SUPPORT
void bind_gui(py::model& m);
#endif

PYBIND11_PLUGIN(dlib)
{
    py::model m("dlib", "dlib python binding");
    // Disable printing of the C++ function signature in the python __doc__ string
    // since it is full of huge amounts of template clutter.
    boost::python::docstring_options options(true,true,false);

#define DLIB_QUOTE_STRING(x) DLIB_QUOTE_STRING2(x)
#define DLIB_QUOTE_STRING2(x) #x

    boost::python::scope().attr("__version__") = DLIB_QUOTE_STRING(DLIB_VERSION);

    bind_matrix(m);
    bind_vector(m);
    bind_svm_c_trainer(m);
    bind_decision_functions(m);
    bind_basic_types(m);
    bind_other(m);
    bind_svm_rank_trainer(m);
    bind_cca(m);
    bind_sequence_segmenter(m);
    bind_svm_struct(m);
    bind_image_classes(m);
    bind_rectangles(m);
    bind_object_detection(m);
    bind_shape_predictors(m);
    bind_correlation_tracker(m);
    bind_face_recognition(m);
#ifndef DLIB_NO_GUI_SUPPORT
    bind_gui(m);
#endif
}

