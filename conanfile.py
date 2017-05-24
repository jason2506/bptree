from conans import ConanFile, CMake


class BPTreeConan(ConanFile):

    name = 'bptree'
    version = '0.0.0'
    url = 'https://github.com/jason2506/bptree-cpp'
    license = 'MIT'
    author = 'Chi-En Wu'

    settings = ('os', 'compiler', 'build_type', 'arch')
    generators = ('cmake', 'txt', 'env')
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
        if self.scope.dev and self.scope.build_tests:
            self.requires('gtest/1.8.0@lasote/stable', private=True)

    def build(self):
        extra_opts = []
        extra_opts.append('-DENABLE_CONAN={}'.format(
            self.options.enable_conan,
        ))
        extra_opts.append('-DBUILD_TESTING={}'.format(
            bool(self.scope.dev and self.scope.build_tests),
        ))
        extra_opts.append('-DCMAKE_INSTALL_PREFIX="{}"'.format(
            self.package_folder,
        ))

        cmake = CMake(self.settings)
        self.run('cmake "{src_dir}" {opts} {extra_opts}'.format(
            src_dir=self.conanfile_directory,
            opts=cmake.command_line,
            extra_opts=' '.join(extra_opts),
        ))
        self.run('cmake --build . {}'.format(cmake.build_config))

    def package(self):
        self.run('cmake --build . --target install')
