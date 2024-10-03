from conans import ConanFile, tools
from conan.tools.cmake import CMake, cmake_layout

class TicketDecoderConan(ConanFile):
   name = 'vrd-extractor'
   version = 'v0.1'
   settings = "os", "compiler", "build_type", "arch"
   generators = "CMakeToolchain", "CMakeDeps"
   options = {}
   requires = [
               ("boost/1.86.0"),
               ("gtest/1.15.0"),
               ("exiv2/0.28.2"),
               ("log4cxx/1.2.0")
               ]
   default_options = {
                # global
                "*:shared": False,
                # boost
                "boost:pch": False,
                "boost:header_only": False,
                "boost:without_atomic": True,
                "boost:without_chrono": True,
                "boost:without_container": True,
                "boost:without_context": True,
                "boost:without_contract": True,
                "boost:without_coroutine": True,
                "boost:without_date_time": True,
                "boost:without_exception": True,
                "boost:without_fiber": True,
                "boost:without_filesystem": True,
                "boost:without_graph": True,
                "boost:without_graph_parallel": True,
                "boost:without_iostreams": True,
                "boost:without_json": True,
                "boost:without_locale": True,
                "boost:without_log": True,
                "boost:without_math": True,
                "boost:without_mpi": True,
                "boost:without_nowide": True,
                "boost:without_program_options": False,
                "boost:without_python": True,
                "boost:without_random": True,
                "boost:without_regex": True,
                "boost:without_serialization": True,
                "boost:without_stacktrace": True,
                "boost:without_system": True,
                "boost:without_test": True,
                "boost:without_thread": True,
                "boost:without_timer": True,
                "boost:without_type_erasure": True,
                "boost:without_url": True,
                "boost:without_wave": True,
                }

   # def configure(self):

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

   def layout(self):
      cmake_layout(self)