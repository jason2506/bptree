from conans import ConanFile, CMake


class BPTreeConan(ConanFile):

    name = 'bptree'
    version = '0.0.0'
    url = 'https://github.com/jason2506/bptree-cpp'
    license = 'MIT'
    author = 'Chi-En Wu'

    settings = ('os', 'compiler', 'build_type', 'arch')
    generators = ('cmake', 'txt')
    no_copy_source = True

    options = {
        'enable_conan': [True, False],
    }
    default_options = (
        'gtest:shared=False',
        'enable_conan=True',
    )

    exports = (
        'CMakeLists.txt',
        'cmake/*.cmake',
        'include/*.hpp',
        'test/CMakeLists.txt',
        'test/*.cpp',
    )

    def requirements(self):
        if self.develop:
            self.requires('gtest/1.8.0@lasote/stable')

    def build(self):
        enable_testing = 'gtest' in self.deps_cpp_info.deps

        cmake = CMake(self)
        cmake.configure(defs={
            'ENABLE_CONAN': self.options.enable_conan,
            'BUILD_TESTING': enable_testing,
        })
        cmake.build()
        if enable_testing:
            cmake.test()

        cmake.install()

    def package(self):
        # files are copied by cmake.install()
        pass

    def package_id(self):
        self.info.header_only()
