# encoding: utf-8
"""
Copyright 2012-2014 Aurélien Gâteau <mail@agateau.com>

Based on code from python-distutils-extra
<https://launchpad.net/python-distutils-extra>:

Copyright 2007, 2008 Sebastian Heinlein
Copyright 2009 Canonical Ltd.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
import distutils
import distutils.command.build
import glob
import os


def setup(**kwargs):
    """
    A version of distutils.core.setup() which takes care of:
    - compiling .po files into .mo files and installing them in the right place.
    - compiling .ui files into ui_*.py files.

    For .po files it assumes the following:

    - all .po files are in the po/ dir.
    - the domain name is the project name.

    It does not require the presence of a POTFILES.in file.

    It does *not* automatically generate a .pot file or update the .po files at
    build time. These two tasks can be done with the update_po command:

        python setup.py update_po

    For .ui files it assumes the following:

    - all .ui files are in the package dirs.
    - code importing .ui files does so with `import ui_<basename>`.

    # Why not use distutils-extra?

    This code is heavily inspired from distutils-extra
    (https://launchpad.net/python-distutils-extra)

    KDE applications do not use _() to mark translated strings: they use
    i18n(), i18nc(), i18np() or i18ncp(). This means we need to pass special
    options to xgettext to generate the .pot file.

    I wanted to use distutils-extra but I could not find how to either pass
    options to xgettext or disable .pot file generation all together. I am also
    not fond of requiring a POTFILES.in file. Therefore I resorted to extract
    the code responsible for compiling the .po and installing them.
    """
    cmdclass = {
        "build": kbuild,
        "build_ui": build_ui,
        "build_i18n": build_i18n,
        "update_po": update_po,
    }
    if "cmdclass" in kwargs:
        # FIXME: Make it possible for the user to override the "build" command?
        kwargs["cmdclass"].update(cmdclass)
    else:
        kwargs["cmdclass"] = cmdclass
    distutils.core.setup(**kwargs)


class build_i18n(distutils.cmd.Command):

    description = "generate .mo files from .po files and add them to the list of files to be installed"

    user_options = []

    def initialize_options(self):
        self.domain = None
        self.po_dir = None

    def finalize_options(self):
        if self.domain is None:
            self.domain = self.distribution.metadata.name
        if self.po_dir is None:
            self.po_dir = "po"

    def run(self):
        """
        Generate mo files and add them to the to be installed files
        """
        if not os.path.isdir(self.po_dir):
            return

        data_files = self.distribution.data_files
        if data_files is None:
            # in case not data_files are defined in setup.py
            self.distribution.data_files = data_files = []

        for po_file in glob.glob("%s/*.po" % self.po_dir):
            lang = os.path.basename(po_file[:-3])
            mo_dir =  os.path.join("build", "mo", lang, "LC_MESSAGES")
            mo_file = os.path.join(mo_dir, "%s.mo" % self.domain)
            if not os.path.exists(mo_dir):
                os.makedirs(mo_dir)

            if _is_outdated(src=po_file, dst=mo_file):
                self.spawn(["msgfmt", po_file, "-o", mo_file])

            targetpath = os.path.join("share/locale", lang, "LC_MESSAGES")
            data_files.append((targetpath, (mo_file,)))


class build_ui(distutils.cmd.Command):
    description = "generate .py files from .ui files"

    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        if self.distribution.packages is None:
            return

        for package in self.distribution.packages:
            if self.distribution.package_dir:
                package_dir = self.distribution.package_dir.get(package, package)
                package_dir = distutils.util.convert_path(package_dir)
            else:
                package_dir = package
            self._build_ui_for_dir(package_dir)

    def _build_ui_for_dir(self, package_dir):
        for ui_file in glob.glob("%s/*.ui" % package_dir):
            name = os.path.basename(ui_file)
            name_without_ext = os.path.splitext(name)[0]
            py_file = os.path.join(package_dir, "ui_%s.py" % name_without_ext)

            if _is_outdated(src=ui_file, dst=py_file):
                self.spawn(["pyuic4", ui_file, "-o", py_file])


class update_po(distutils.cmd.Command):
    description = "update .pot and .po files"

    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def _list_source_files(self, exts):
        lst = []
        IGNORED_DIRS = [".git", ".svn", ".bzr", "build", "dist"]
        for root, dirs, files in os.walk("."):
            for ignored_dir in IGNORED_DIRS:
                if ignored_dir in dirs:
                    dirs.remove(ignored_dir)
            src = [os.path.join(root, x) for x in files if os.path.splitext(x)[1] in exts]
            lst.extend(src)
        return lst

    def run(self):
        name = self.distribution.metadata.name
        po_dir = "po"

        # Generate .pot file
        pot_file = "%s/%s.pot" % (po_dir, name)
        keywords = [
            "i18n",
            "i18nc:1c,2",
            "i18np:1,2",
            "i18ncp:1c,2,3",
            "ki18n"]

        src_files = self._list_source_files([".py"])
        cmd = ["xgettext"] \
            + ["-k" + x for x in keywords] \
            + ["--language=Python", "-o", pot_file] \
            + src_files
        self.spawn(cmd)

        # Update .po files
        for po in glob.glob("%s/*.po" % po_dir):
            cmd = ["msgmerge", "--backup=none", "--update", po, pot_file]
            self.spawn(cmd)


class kbuild(distutils.command.build.build):
    """
    Make sure build_i18n and build_ui are called when building
    """
    def finalize_options(self):
        distutils.command.build.build.finalize_options(self)
        self.sub_commands.append(("build_i18n", lambda x: True))
        self.sub_commands.append(("build_ui", lambda x: True))


def _is_outdated(src, dst):
    src_mtime = os.path.getmtime(src)
    dst_mtime = os.path.exists(dst) and os.path.getmtime(dst) or 0
    return src_mtime > dst_mtime
