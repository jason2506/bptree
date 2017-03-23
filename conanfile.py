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
        'enable_conan=True',
    )

    exports = (
        'CMakeLists.txt',
        'cmake/*.cmake',
        'include/*.hpp',
    )

    def build(self):
        extra_opts = []
        extra_opts.append('-DENABLE_CONAN={}'.format(
            self.options.enable_conan,
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
