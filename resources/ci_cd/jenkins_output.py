#!/usr/bin/env python3
#
# (c) 2021 Copyright, Real-Time Innovations, Inc.  All rights reserved. RTI
# grants Licensee a license to use, modify, compile, and create derivative
# works of the Software.  Licensee has the right to distribute object form only
# for use with RTI products.  The Software is provided "as is", with no
# warranty of any type, including any warranty for fitness for any purpose. RTI
# is under no obligation to maintain or support the Software.  RTI shall not be
# liable for any incidental or consequential damages arising out of the use or
# inability to use the software.

"""
Creates a Markdown file with the output of the Jenkins stages
"""

import sys
import platform
import subprocess


def main():
    if len(sys.argv) != 2:
        sys.exit(f"Usage: {sys.argv[0]} <RTI Connext DDS version>")

    text = (
        "This build is being executed on an internal Jenkins, only RTI "
        "employers can access the build logs. To give information to external "
        "users, we have configured the status checks.\n\n"
        "# Pipeline description\n"
        "The pipeline is defined "
        "[here](https://github.com/rticommunity/rticonnextdds-examples/blob/develop/Jenkinsfile) "
        "and the purpose of this pipeline is to build the examples using the "
        "last staging packages of RTI Connext® DDS and to analyze the build "
        " using `analyze-build` from `clang-tools`.\n\n"
        "# Environment details\n"
        f"| Option                   | Setting              |\n"
        f"| ------------------------ | -------------------- |\n"
        f"| RTI Connext® DDS Version | {sys.argv[1]}        |\n"
        f"| System                   | {platform.system()}  |\n"
        f"| Machine type             | {platform.machine()} |\n"
        f"| OS release               | {platform.release()} |\n"
    )

    if platform.system() == "Linux":
        gcc_version = (
            subprocess.run(["gcc", "-dumpfullversion"], capture_output=True)
            .stdout[:-1]
            .decode("utf-8")
        )
        text += f"| GCC Version              | {gcc_version}         |\n"

        clang_version = (
            subprocess.run(["clang", "-dumpversion"], capture_output=True)
            .stdout[:-1]
            .decode("utf-8")
        )
        text += f"| CLANG Version            | {clang_version}       |\n"
    elif platform.system() == "Darwin":
        output = subprocess.run(
            ["clang", "--version"], capture_output=True
        ).stdout.decode("utf-8")
        clang_version = output.split("\n")[0]

        text += f"| CLANG Version            | {clang_version}       |\n"

    with open("Jenkinsfile", "r") as file:
        text += "\n<details><summary>Jenkinsfile</summary>\n<p>\n"
        jenkinsfile = file.read()
        text += f"\n```Groovy\n{jenkinsfile}\n```\n"
        text += "</p>\n</details>"

    with open("jenkins_output.md", "w") as file:
        file.write(text)


if __name__ == "__main__":
    main()
